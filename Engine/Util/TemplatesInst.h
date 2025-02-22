#pragma once
#ifdef TEMPLATES_INST
#define NW_TI_VECTOR
#define NW_TI_MAP
#define NW_TI_UNORDERED_HASHMAP
#define NW_TI_DEQUE 
#define NW_TI_LIST
#define TEMPLATES_INST_IMPL(type) template class type;
#else
#define TEMPLATES_INST_IMPL(type) extern template class type;
#endif

#ifdef NW_TI_VECTOR
#include <vector>
TEMPLATES_INST_IMPL(std::vector<char*>)
TEMPLATES_INST_IMPL(std::vector<int>)
TEMPLATES_INST_IMPL(std::vector<std::string>)
TEMPLATES_INST_IMPL(std::vector<float>)
#endif

#ifdef NW_TI_MAP
#include <map>
#endif

#ifdef NW_TI_UNORDERED_HASHMAP
#include <unordered_map>
#endif

#ifdef NW_TI_DEQUE
#include <deque>
#endif

#ifdef NW_TI_LIST
#include <list>
#endif
