#include <stdio.h>
#include <string.h>

#include "conf_file_process.h"
#include "util/util.h"

ConfFileProcessor * ConfFileProcessor::instance = nullptr;

ConfFileProcessor::ConfFileProcessor(){}

ConfFileProcessor::~ConfFileProcessor(){
    for(auto & confItem : m_confItemList){
        delete confItem;
    }
}

bool ConfFileProcessor::load(const char * confFileName){
    FILE * fp;
    fp = fopen(confFileName, "r");
    if(!fp){
        return false;
    }

    char lineBuf[CONF_LINE_SIZE];
    while(!feof(fp)){
        if(fgets(lineBuf, CONF_LINE_SIZE, fp) == nullptr){
            continue;
        }

        if(strlen(lineBuf) == 0){
            continue;
        }

        if(lineBuf[0] == '#' || lineBuf[0] == '[' || lineBuf[0] == ' ' || lineBuf[0] == '\t' || lineBuf[0] == '\n' || lineBuf[0] == ';'){
            continue;
        }

procString:
        size_t len = strlen(lineBuf);
        if(lineBuf[len - 1] == '\r' || lineBuf[len - 1] == '\n' || lineBuf[len - 1] == ' '){
            lineBuf[len - 1] = 0;
            goto procString;
        }

        char * pos = strchr(lineBuf, '=');
        if(pos != NULL){
            ConfItem * confItem = new ConfItem;
            memset(confItem,0,sizeof(ConfItem));
            strncpy(confItem->itemName, lineBuf, (int)(pos - lineBuf));
            strcpy(confItem->itemContent, pos + 1);

            Rtrim(confItem->itemName);
			Ltrim(confItem->itemName);
			Rtrim(confItem->itemContent);
			Ltrim(confItem->itemContent);

            m_confItemList.push_back(confItem);
        }
    }

    fclose(fp);
    return true;
}

// 根据itemName获取配置信息字符串
const char * ConfFileProcessor::getItemContent_str(const char * itemName){
    for(auto & confItem : m_confItemList){
        if(strcasecmp(confItem->itemName, itemName) == 0){
            return confItem->itemContent;
        }
    }

    return nullptr;
}

// 根据ItemName获取数字类型配置信息
int ConfFileProcessor::getItemContent_int(const char * itemName, const int def){
    for(auto & confItem : m_confItemList){
        if(strcasecmp(confItem->itemName, itemName) == 0){
            return atoi(confItem->itemContent);
        }
    }

    return def;
}
