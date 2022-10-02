#ifndef GRIDOPERATION_H
#define GRIDOPERATION_H

#include "FBParser.h"
#include <wx/string.h>
#include <wx/log.h>

#include <concepts>

namespace GridOperation { 

template<typename T>
concept GridOperatable = requires (T* grid)
{

    { grid->GetNumberRows() } -> std::convertible_to<int>;
    { grid->GetNumberCols() } -> std::convertible_to<int>;
    
    { grid->SetValue(0, 0, grid->GetValue(0, 0)) };
 
    { grid->AppendRows(0) }    -> std::same_as<bool>;
    { grid->InsertRows(0, 0) } -> std::same_as<bool>;
    { grid->DeleteRows(0, 0) } -> std::same_as<bool>;

};

template<typename T, typename U>
requires GridOperatable<T> && GridOperatable<U>
inline void copy(T* src, U* dst)
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

    static std::remove_cvref_t<decltype(src->GetValue(0, 0))> value;
    for(decltype(num_rows) row = 0; row < num_rows; row++)  
    {
        for(decltype(num_cols) col = 0; col < num_cols; col++)
        {
            value = src->GetValue(row, col);
            dst->SetValue(row, col, value);
        }
    }
}

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

template<typename T, typename U>
requires GridOperatable<T> && GridOperatable<U>
inline void insert(T* src, U* dst, decltype(dst->GetNumberRows()) pos)
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

    if(pos > dst->GetNumberRows())
    {
        wxLogMessage("pos > dst->GetNumberRows()");
        return;
    }

    dst->InsertRows(pos, num_rows);

    static std::remove_cvref_t<decltype(src->GetValue(0, 0))> value;
    for(decltype(num_rows) row = 0; row < num_rows; row++)  
    {
        for(decltype(num_cols) col = 0; col < num_cols; col++)
        {
            value = src->GetValue(row, col);
            dst->SetValue(row + pos, col, value);
        }
    }


}

/*
template<typename T, typename N = decltype(T::GetNumberRows())>
requires GridOperatable<T> && std::integral<N>
inline void swap_row(T* grid, N pos1, N pos2)
{
    auto num_cols = grid->GetNumberCols();

    if(pos1 >= grid->GetNumberRows())
    {
        wxLogMessage("pos1 >= grid->GetNumberRows()");
        return;
    }

    if(pos2 >= grid->GetNumberRows())
    {
        wxLogMessage("pos2 >= grid->GetNumberRows()");
        return;
    }

    static decltype(grid->GetValue(0, 0)) tmp_buff;

    for(decltype(num_cols) col = 0; col < num_cols; col++)
    {
        tmp_buff = grid->GetValue(pos2, col);

        auto&& value = grid->GetValue(pos1, col);
        grid->SetValue(pos2, col, value);

        grid->SetValue(pos1, col, tmp_buff);
    }
}
*/


class FBGrid
{
public:
    FBGrid(FBParser& fb) : m_fb(fb){};
    ~FBGrid(){};

    FBGrid& set_current_part(FBPart part)
    {
        m_fb.set_current_part(part);
        return *this;
    }

    //Adapter Function (like wxGridTableBase)
    int GetNumberRows() const { return m_fb.get_number_rows(); };
    int GetNumberCols() const { return m_fb.get_number_cols(); };

    const wxString& GetValue(int row, int col)
    {
        auto value = m_fb.get_value(row, col);
        m_wxstring_buff.assign(value.data(), value.size());
        return m_wxstring_buff;
    };

    bool SetValue(int row, int col, const wxString& value)
    {
        m_string_buff = value;
        return m_fb.set_value(row, col, m_string_buff);
    };

    bool AppendRows(int numRows = 1){ return m_fb.append_rows(numRows); };
    bool InsertRows(int pos = 0, int numRows = 1){ return m_fb.insert_rows(pos, numRows); };
    bool DeleteRows(int pos = 0, int numRows = 1){ return m_fb.delete_rows(pos, numRows); };

private:
    FBParser& m_fb;

    wxString m_wxstring_buff;
    std::string m_string_buff;
};

}

#endif //GRIDOPERATION
