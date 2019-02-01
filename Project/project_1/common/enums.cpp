#include "./enums.hpp"

string get_atr_message(AttributeType type)
{
    switch (type)
    {
    case INPUT:
        return "Give me an Input File please:";
    case OUTPUT:
        return "Give me an Output File please:";
    case QUERY:
        return "Give me a Query File please:";
    case LS_COUNT:
        return "Give me the k parameter(locality-sensitive functions h) please:";
    case HASH_TABLES:
        return "Give me the Hash Tables (L) count please:";
    case C_LS_COUNT:
        return "Give me the k parameter(locality-sensitive functions h) please:";
    case M_COUNT:
        return "Give me the max check points (M) please:";
    case PROBES:
        return "Give me the probes please:";
    default:
        return "";
    }
}