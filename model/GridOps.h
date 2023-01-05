#ifndef GRID_OPS_H
#define GRID_OPS_H

#include "FBParser.h"

#include <wx/grid.h>
#include <wx/string.h>
#include <wx/log.h>

#include <concepts>

namespace GridOps {

using siz_t = long long;
using val_t = wxString;

template<typename T>
class Adapter{};

template<>
class Adapter<wxGridTableBase>
{
public:
    Adapter(wxGrid* wxgrid){ m_wxgridTable = wxgrid->GetTable(); };
    Adapter(wxGrid& wxgrid){ m_wxgridTable = wxgrid.GetTable(); };

    Adapter(wxGridTableBase* wxgridTable){ m_wxgridTable = wxgridTable; };
    Adapter(wxGridTableBase& wxgridTable){ m_wxgridTable = &wxgridTable; };

    //Adapter Function
    siz_t GetNumberRows() const { return m_wxgridTable->GetNumberRows(); };
    siz_t GetNumberCols() const { return m_wxgridTable->GetNumberCols(); };   

    const val_t& GetValue(siz_t row, siz_t col)
    {
        m_wxstrBuff = m_wxgridTable->GetValue(row, col);
        return m_wxstrBuff;
    };

    void SetValue(siz_t row, siz_t col, const val_t& value)
    { 
        m_wxstrBuff = value;
        m_wxgridTable->SetValue(row, col, m_wxstrBuff);
    };

    bool AppendRows(siz_t numRows = 1){ return m_wxgridTable->AppendRows(numRows); };
    bool InsertRows(siz_t pos = 0, siz_t numRows = 1){ return m_wxgridTable->InsertRows(pos, numRows); };
    bool DeleteRows(siz_t pos = 0, siz_t numRows = 1){ return m_wxgridTable->DeleteRows(pos, numRows); };

private:
    wxGridTableBase* m_wxgridTable;
    wxString m_wxstrBuff;
};

template<>
class Adapter<FBGrid>
{
public:
    Adapter(FBGrid* fbgrid){ m_fbgrid = fbgrid; };
    Adapter(FBGrid& fbgrid){ m_fbgrid = &fbgrid; };
    ~Adapter(){};

    //Adapter Function
    siz_t GetNumberRows() const { return m_fbgrid->get_number_rows(); };
    siz_t GetNumberCols() const { return m_fbgrid->get_number_cols(); };

    const val_t& GetValue(siz_t row, siz_t col)
    {
        m_wxstrBuff = m_fbgrid->get_value(row, col);
        return m_wxstrBuff;
    };

    void SetValue(siz_t row, siz_t col, const val_t& value)
    {
        m_stdstrBuff = value;
        m_fbgrid->set_value(row, col, m_stdstrBuff);
    };

    bool AppendRows(siz_t numRows = 1){ return m_fbgrid->append_rows(numRows); };
    bool InsertRows(siz_t pos = 0, siz_t numRows = 1){ return m_fbgrid->insert_rows(pos, numRows); };
    bool DeleteRows(siz_t pos = 0, siz_t numRows = 1){ return m_fbgrid->delete_rows(pos, numRows); };

private:
    FBGrid* m_fbgrid;
    wxString m_wxstrBuff;
    std::string m_stdstrBuff;
};

template<typename T>
using rm_ptrcvref_t = std::remove_pointer_t<std::remove_cvref_t<T>>;

template<typename T>
Adapter(T) -> Adapter<rm_ptrcvref_t<T>>;

template<typename T>
requires 
    std::derived_from<rm_ptrcvref_t<T>, wxGridTableBase> ||
    std::derived_from<rm_ptrcvref_t<T>, wxGrid>
Adapter(T) -> Adapter<wxGridTableBase>;

template<typename T>
requires std::derived_from<rm_ptrcvref_t<T>, FBGrid>
Adapter(T) -> Adapter<FBGrid>;


template<typename T>
concept GridOperatable = requires (T x, siz_t s, val_t v)
{
    { x.GetNumberRows() } -> std::same_as<siz_t>;
    { x.GetNumberCols() } -> std::same_as<siz_t>;
    
    { x.GetValue(s, s) } -> std::same_as<const val_t&>;
    { x.SetValue(s, s, v) };
 
    { x.AppendRows(s) }    -> std::same_as<bool>;
    { x.InsertRows(s, s) } -> std::same_as<bool>;
    { x.DeleteRows(s, s) } -> std::same_as<bool>;
};

template<typename T, typename U>
requires GridOperatable<T> && GridOperatable<U>
inline void copy(T& src, U& dst)
{
    if((void*)&src == (void*)&dst)
    {
        wxLogMessage("(void*)&src == (void*)&dst");
        return;      
    }

    if(src.GetNumberCols() != dst.GetNumberCols())
    {
        wxLogMessage("src.GetNumberCols() != dst.GetNumberCols()");
        return;
    }

    auto numRows = src.GetNumberRows();
    auto numCols = src.GetNumberCols();

    if(dst.GetNumberRows() != 0)
    {
        dst.DeleteRows(0, dst.GetNumberRows());
    }

    dst.AppendRows(numRows);

    for(decltype(numRows) row = 0; row < numRows; ++row)  
    {
        for(decltype(numCols) col = 0; col < numCols; ++col)
        {
            auto&& value = src.GetValue(row, col);
            dst.SetValue(row, col, value);
        }
    }
}

template<typename T, typename U>
requires GridOperatable<T> && GridOperatable<U>
inline void insert(T& src, U& dst, siz_t pos)
{
    if((void*)&src == (void*)&dst)
    {
        wxLogMessage("(void*)&src == (void*)&dst");
        return;      
    }

    if(src.GetNumberCols() != dst.GetNumberCols())
    {
        wxLogMessage("src.GetNumberCols() != dst.GetNumberCols()");
        return;
    }

    auto numRows = src.GetNumberRows();
    auto numCols = src.GetNumberCols();

    if(pos > dst.GetNumberRows())
    {
        wxLogMessage("pos > dst.GetNumberRows()");
        return;
    }

    dst.InsertRows(pos, numRows);

    for(decltype(numRows) row = 0; row < numRows; ++row)  
    {
        for(decltype(numCols) col = 0; col < numCols; ++col)
        {
            auto&& value = src.GetValue(row, col);
            dst.SetValue(row + pos, col, value);
        }
    }

}

template<typename T, typename U>
inline void append(T& src, U& dst)
{
    insert(src, dst, dst->GetNumberRows());
}


} // namespace GridOps

#endif // GRID_OPS_H