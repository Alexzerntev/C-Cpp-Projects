#include "./settings.h"

void destroy_settings(Settings *settings, int is_partial)
{
    free(settings->transactions_file);
    settings->transactions_file = NULL;

    free(settings->bitcoin_balances_file);
    settings->bitcoin_balances_file = NULL;

    if (!is_partial)
    {
        free(settings);
    }
    return;
}