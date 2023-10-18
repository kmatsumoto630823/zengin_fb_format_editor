#ifndef GRID_OPS_H
#define GRID_OPS_H

#include "FBData.h"

#include <wx/grid.h>
#include <wx/string.h>
#include <wx/log.h>

#include <concepts>

namespace GridOps {

using siz_t = long long;
using val_t = wxString;


template<typename T>
using rm_ptrcvref_t = std::remove_pointer_t<std::remove_cvref_t<T>>;

template<typename T, typename U>
concept derived_from_unmod = std::derived_from<rm_ptrcvref_t<T>, U>;


template<typename T>
class Adapter{};

template<>
class Adapter<wxGridTableBase>
{
public:
    using size_type  = siz_t;
    using value_type = val_t;

    Adapter(wxGrid* wxgrid){ m_wxgridTable = wxgrid->GetTable(); };
    Adapter(wxGrid& wxgrid){ m_wxgridTable = wxgrid.GetTable(); };

    Adapter(wxGridTableBase* wxgridTable){ m_wxgridTable = wxgridTable; };
    Adapter(wxGridTableBase& wxgridTable){ m_wxgridTable = &wxgridTable; };
    

    //Adapter Function
    size_type GetNumberRows() const { return m_wxgridTable->GetNumberRows(); };
    size_type GetNumberCols() const { return m_wxgridTable->GetNumberCols(); };   

    const value_type& GetValue(size_type row, size_type col)
    {
        m_wxstrBuff = m_wxgridTable->GetValue(row, col);
        return m_wxstrBuff;
    };

    void SetValue(size_type row, size_type col, const value_type& value)
    { 
        m_wxstrBuff = value;
        m_wxgridTable->SetValue(row, col, m_wxstrBuff);
    };

    bool AppendRows(size_type numRows = 1){ return m_wxgridTable->AppendRows(numRows); };
    bool InsertRows(size_type pos = 0, size_type numRows = 1){ return m_wxgridTable->InsertRows(pos, numRows); };
    bool DeleteRows(size_type pos = 0, size_type numRows = 1){ return m_wxgridTable->DeleteRows(pos, numRows); };

private:
    wxGridTableBase* m_wxgridTable;
    value_type m_wxstrBuff;
};

template<typename T>
requires derived_from_unmod<T, wxGridTableBase> || derived_from_unmod<T, wxGrid>
Adapter(T) -> Adapter<wxGridTableBase>;


template<>
class Adapter<FBGrid>
{
public:
    using size_type  = siz_t;
    using value_type = val_t;

    Adapter(FBGrid* fbgrid){ m_fbgrid = fbgrid; };
    Adapter(FBGrid& fbgrid){ m_fbgrid = &fbgrid; };

    //Adapter Function
    size_type GetNumberRows() const { return m_fbgrid->get_number_rows(); };
    size_type GetNumberCols() const { return m_fbgrid->get_number_cols(); };

    const value_type& GetValue(size_type row, size_type col)
    {
        m_wxstrBuff = m_fbgrid->get_value(row, col);
        return m_wxstrBuff;
    };

    void SetValue(size_type row, size_type col, const value_type& value)
    {
        m_stdstrBuff = value;
        m_fbgrid->set_value(row, col, m_stdstrBuff);
    };

    bool AppendRows(size_type numRows = 1){ return m_fbgrid->append_rows(numRows); };
    bool InsertRows(size_type pos = 0, size_type numRows = 1){ return m_fbgrid->insert_rows(pos, numRows); };
    bool DeleteRows(size_type pos = 0, size_type numRows = 1){ return m_fbgrid->delete_rows(pos, numRows); };

private:
    FBGrid* m_fbgrid;

    value_type m_wxstrBuff;
    std::string m_stdstrBuff;
};

template<typename T>
requires derived_from_unmod<T, FBGrid>
Adapter(T) -> Adapter<FBGrid>;


template<typename T>
concept GridOperatable = requires (T t, typename T::size_type s, typename T::value_type v)
{
    { t.GetNumberRows() } -> std::same_as<typename T::size_type>;
    { t.GetNumberCols() } -> std::same_as<typename T::size_type>;
    
    { t.GetValue(s, s) } -> std::same_as<const typename T::value_type&>;
    { t.SetValue(s, s, v) };
 
    { t.AppendRows(s) }    -> std::same_as<bool>;
    { t.InsertRows(s, s) } -> std::same_as<bool>;
    { t.DeleteRows(s, s) } -> std::same_as<bool>;
};

template<typename T, typename U>
requires GridOperatable<T> && GridOperatable<U>
inline void Copy(T& src, U& dst)
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
inline void Insert(T& src, U& dst, siz_t pos)
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
inline void Append(T& src, U& dst)
{
    Insert(src, dst, dst->GetNumberRows());
}

} // namespace GridOps

#endif // GRID_OPS_H