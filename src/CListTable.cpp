/*
 * CListTable.cpp
 *
 *  Created on: Jun 6, 2014
 *      Author: root
 */
#include <stdio.h>
#include <list.h>
#include <list_def.h>
#include <CListTable.h>

int CListTable::OpenDir(list_data* list, extetype exte_type, char* comp_path)
{
    int id = list_get_id_by_comp_path(list, comp_path);

    if(id == 0)
        return -1;

    CListTable::dirct_id = id;
    CListTable::list = list;
    CListTable::exte_type = exte_type;
    CListTable::count = list_get_extetype_count_folder(list, exte_type, id);
}

void CloseDir()
{

}

int CListTable::GetCount()
{
    return CListTable::count;
}

const char* CListTable::GetFile(int index)
{
    return list_get_file_name_folder(CListTable::list, CListTable::exte_type, CListTable::dirct_id, index);
}

const char* CListTable::GetItem(int index)
{

}

int CListTable::GetFileIndex(char* comp_path)
{

}

const char* CListTable::GetParent()
{
    return list_get_parent_comp_path_by_id(CListTable::list, CListTable::dirct_id);
}

extetype CListTable::GetType()
{
    return CListTable::exte_type;
}

CListTable::CListTable()
{
    // TODO Auto-generated constructor stub

}

CListTable::~CListTable()
{
    // TODO Auto-generated destructor stub
}
