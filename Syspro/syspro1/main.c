#include <stdio.h>
#include "./io/io_manager.h"
#include "./io/user_interaction.h"
#include "./model/settings/settings.h"
#include "./error_handler/error_handler.h"

int main(int argc, char *argv[])
{
    int result_code = 0;
    char last_date[11] = "11-11-1111";
    char last_time[6] = "11:11";
    int last_transaction_id = 0;
    Hashtable *bitcoin_hashtable = NULL;
    Hashtable *receiver_hashtable = NULL;
    Hashtable *sender_hashtable = NULL;

    BitcoinTreeNode **all_bitcoins = NULL;
    int all_bitcoins_sum = 0;
    Settings *settings = (Settings *)malloc(sizeof(Settings));

    if (settings == NULL)
    {
        print_error(MEMORY_ALLOCATION_ERROR, "Settings malloc");
        exit(1);
    }

    result_code = validate_and_store(argv, argc, settings);
    if (result_code != SUCCESS)
    {
        print_error(WRONG_INITIALIZATION_ERROR, "Wrong arguments");
        exit(1);
    }

    all_bitcoins = get_all_of_bitcoins(settings->bitcoin_balances_file, &result_code);

    if (result_code != SUCCESS)
    {
        print_error(WRONG_INITIALIZATION_ERROR, "All bitcoins map");
        exit(1);
    }

    bitcoin_hashtable = read_bitcoin_balances_file(settings->bitcoin_balances_file, settings, &result_code, all_bitcoins, &all_bitcoins_sum);
    if (result_code != SUCCESS)
    {
        print_error(WRONG_INITIALIZATION_ERROR, "Read bitcoin balances");
        exit(1);
    }

    result_code = read_transactions_file(settings->transactions_file, settings, &receiver_hashtable, &sender_hashtable, bitcoin_hashtable, last_date, last_time, &last_transaction_id);
    if (result_code != SUCCESS)
    {
        print_error(WRONG_INITIALIZATION_ERROR, "Read transactions file");
        exit(1);
    }

    interact(bitcoin_hashtable, receiver_hashtable, sender_hashtable, settings, all_bitcoins, all_bitcoins_sum, last_transaction_id, last_date, last_time);

    // house keeping
    free(all_bitcoins);
    destroy_settings(settings, 0);
    destroy_hashtable(bitcoin_hashtable, 1);
    destroy_hashtable(receiver_hashtable, 1);
    destroy_hashtable(sender_hashtable, 0);

    return 0;
}