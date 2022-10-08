#ifndef GRID_OPS_H
#define GRID_OPS_H

#include "FBParser.h"

#include <wx/grid.h>
#include <wx/string.h>
#include <wx/log.h>

#include <concepts>

namespace GridOps { 

template<typename T>
concept GridOperatable = requires (T* grid)
{
    typename T::val_t;
    typename T::siz_t;

    { grid->GetNumberRows() } -> std::convertible_to<int>;
    { grid->GetNumberCols() } -> std::convertible_to<int>;
    
    { grid->SetValue(0, 0, grid->GetValue(0, 0)) };
 
    { grid->AppendRows(0) }    -> std::same_as<bool>;
    { grid->InsertRows(0, 0) } -> std::same_as<bool>;
    { grid->DeleteRows(0, 0) } -> std::same_as<bool>;

};

template<typename T, typename U>
concept ValueConvertible = requires (T* grid1, U* grid2)
{
    { grid1->SetValue(0, 0, grid2->GetValue(0, 0)) };
    { grid2->SetValue(0, 0, grid1->GetValue(0, 0)) };
};

template<typename T, typename U>
requires GridOperatable<T> && GridOperatable<U> && ValueConvertible<T, U>
void copy(T* src, U* dst)
{
    using val_t = T::val_t;
    using siz_t = T::siz_t;

    if((void*)src == (void*)dst)
    {
        wxLogMessage("(void*)src == (void*)dst");
        return;      
    }

    if(src->GetNumberCols() != dst->GetNumberCols())
    {
        wxLogMessage("src->GetNumberCols() != dst->GetNumberCols()");
        return;
    }

    auto num_rows = src->GetNumberRows();
    auto num_cols = src->GetNumberCols();

    if(dst->GetNumberRows() != 0)
    {
        dst->DeleteRows(0, dst->GetNumberRows());
    }

    dst->AppendRows(num_rows);

    static val_t value_buff;
    for(siz_t row = 0; row < num_rows; ++row)  
    {
        for(siz_t col = 0; col < num_cols; ++col)
        {
            value_buff = src->GetValue(row, col);
            dst->SetValue(row, col, value_buff);
        }
    }
}

template<typename T, typename U, typename P>
requires GridOperatable<T> && GridOperatable<U> && ValueConvertible<T, U>
void copy_if(T* src, U* dst, P pred)
{
    using val_t = T::val_t;
    using siz_t = T::siz_t;

    if((void*)src == (void*)dst)
    {
        wxLogMessage("(void*)src == (void*)dst");
        return;      
    }

    if(src->GetNumberCols() != dst->GetNumberCols())
    {
        wxLogMessage("src->GetNumberCols() != dst->GetNumberCols()");
        return;
    }

    auto num_rows = src->GetNumberRows();
    auto num_cols = src->GetNumberCols();

    if(dst->GetNumberRows() != 0)
    {
        dst->DeleteRows(0, dst->GetNumberRows());
    }

    dst->AppendRows(num_rows);

    static val_t value_buff;
    size_t num_copy = 0;
    for(siz_t row = 0; row < num_rows; ++row)  
    {
        for(siz_t col = 0; col < num_cols; ++col)
        {
            value_buff = src->GetValue(row, col);
            dst->SetValue(row, col, value_buff);
        }
    }    
}

template<typename T, typename U, typename I = U::siz_t>
requires GridOperatable<T> && GridOperatable<U> && ValueConvertible<T, U> && std::integral<I>
void insert(T* src, U* dst, I pos)
{
    using val_t = T::val_t;
    using siz_t = T::siz_t;

    if((void*)src == (void*)dst)
    {
        wxLogMessage("(void*)src == (void*)dst");
        return;      
    }

    if(src->GetNumberCols() != dst->GetNumberCols())
    {
        wxLogMessage("src->GetNumberCols() != dst->GetNumberCols()");
        return;
    }

    auto num_rows = src->GetNumberRows();
    auto num_cols = src->GetNumberCols();

    if(pos > dst->GetNumberRows())
    {
        wxLogMessage("pos > dst->GetNumberRows()");
        return;
    }

    dst->InsertRows(pos, num_rows);

    static val_t value_buff;
    for(siz_t row = 0; row < num_rows; ++row)  
    {
        for(siz_t col = 0; col < num_cols; ++col)
        {
            value_buff = src->GetValue(row, col);
            dst->SetValue(row + pos, col, value_buff);
        }
    }

}

template<typename T, typename U>
void append(T* src, U* dst)
{
    insert(src, dst, dst->GetNumberRows());
}


template<typename T>
class Adapter
{
    //DUMMY PRIMARY TEMPLATE
};

template<typename T>
requires std::derived_from<T, wxGrid>
class Adapter<T>
{
public:
    using val_t = wxString;
    using siz_t = int;

    Adapter(T* grid){ m_wxgrid_table = grid->GetTable(); };
    ~Adapter(){};

    //Adapter Function
    int GetNumberRows() const { return m_wxgrid_table->GetNumberRows(); };
    int GetNumberCols() const { return m_wxgrid_table->GetNumberCols(); };   

    const wxString& GetValue(int row, int col)
    {
        m_wxstr_buff = m_wxgrid_table->GetValue(row, col);
        return m_wxstr_buff;
    };

    void SetValue(int row, int col, const wxString& value)
    { 
        m_wxgrid_table->SetValue(row, col, value);
    };

    bool AppendRows(int numRows = 1){ return m_wxgrid_table->AppendRows(numRows); };
    bool InsertRows(int pos = 0, int numRows = 1){ return m_wxgrid_table->InsertRows(pos, numRows); };
    bool DeleteRows(int pos = 0, int numRows = 1){ return m_wxgrid_table->DeleteRows(pos, numRows); };

private:
    wxGridTableBase* m_wxgrid_table;
    wxString m_wxstr_buff;

};


template<>
class Adapter<FBParser>
{
public:
    using val_t = wxString;
    using siz_t = int;

    Adapter(FBParser* fbgrid){ m_fbgrid = fbgrid; };
    ~Adapter(){};

    void set_current_part(FBPart part)
    {
        m_fbgrid->set_current_part(part);
    }

    Adapter& operator[](FBPart part)
    {
        set_current_part(part);
        return *this;
    }

    //Adapter Function
    int GetNumberRows() const { return m_fbgrid->get_number_rows(); };
    int GetNumberCols() const { return m_fbgrid->get_number_cols(); };

    const wxString& GetValue(int row, int col)
    {
        auto value = m_fbgrid->get_value(row, col);
        m_wxstr_buff.assign(value.data(), value.size());
        return m_wxstr_buff;
    };

    void SetValue(int row, int col, const wxString& value)
    {
        m_stdstr_buff = value;
        m_fbgrid->set_value(row, col, m_stdstr_buff);
    };

    bool AppendRows(int numRows = 1){ return m_fbgrid->append_rows(numRows); };
    bool InsertRows(int pos = 0, int numRows = 1){ return m_fbgrid->insert_rows(pos, numRows); };
    bool DeleteRows(int pos = 0, int numRows = 1){ return m_fbgrid->delete_rows(pos, numRows); };

private:
    FBParser* m_fbgrid;

    wxString m_wxstr_buff;
    std::string m_stdstr_buff;
};


template<typename T>
Adapter(T*) -> Adapter<T>;

} // namespace GridOps

#endif // GRID_OPS_H

/*
template<typename T, typename U>
inline void copy(T* src, U* dst, wxProgressDialog* dlg)
{
    if((void*)src == (void*)dst)
    {
        wxLogMessage("srt == dst");
        return;      
    }

    if(src->GetNumberCols() != dst->GetNumberCols())
    {
        wxLogMessage("src->GetNumberCols() != dst->GetNumberCols()");
        return;
    }

    auto num_rows = src->GetNumberRows();
    auto num_cols = src->GetNumberCols();

    if(dst->GetNumberRows() != 0)
    {
        dst->DeleteRows(0, dst->GetNumberRows());
    }
    
    dst->AppendRows(num_rows);

    wxString msg;
    auto update_msg = [&msg, &dlg](auto row, auto num_rows)
    {
        msg.Empty();
        msg <<  row * 100 / num_rows << "% ";
        msg << "( " << row  << " / " << num_rows << " )";
        dlg->SetRange(num_rows);
        dlg->Update(row, msg);
    };

    for(decltype(num_rows) row = 0; row < num_rows; row++)  
    {
       
        if(row % (num_rows / 100 + 1) == 0)
        {
            update_msg(row, num_rows);
        }

        for(decltype(num_cols) col = 0; col < num_cols; col++)
        {
            auto&& value = src->GetValue(row, col);
            dst->SetValue(row, col, value);
        }
    }

    update_msg(num_rows, num_rows);
}
*/
