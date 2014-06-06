/*
 * CListTable.h
 *
 *  Created on: Jun 6, 2014
 *      Author: root
 */

#ifndef CLISTTABLE_H_
#define CLISTTABLE_H_

#include <list.h>

class CListTable
{
    public:
        int OpenDir(list_data* list, extetype exte_type, char* comp_path);
        void CloseDir();

        int GetCount();
        const char* GetFile(int index);
        const char* GetItem(int index);

        int GetFileIndex(char* comp_path);

        const char* GetParent();

        extetype GetType();

        CListTable();
        virtual ~CListTable();

    private:
        int count;
        int dirct_id;
        extetype exte_type;
        list_data* list;
};

#endif /* CLISTTABLE_H_ */
