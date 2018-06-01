#include "test.h"

int main(int argc, char** argv)
{
    // Parse args
    hash_input_t hash_input = {};
    int err = parse_args(argc, argv, true, &hash_input);
    if (err != 0) {
        exit(1);
    }

    // Hash low level
    hash_result_t hash_result = hash_low_level(&hash_input);
    if(hash_result.err != ARGON2_OK) {
        fprintf(
            stdout,
            "Argon2 error: %s\n",
            argon2_error_message(hash_result.err)
        );
        exit(1);
    }

    // Verify low level
    verify_input_t verify_input = {};
    verify_input.encoded        = hash_result.encoded;
    verify_input.password       = hash_input.password;
    verify_input.password_len   = hash_input.password_len;
    verify_input.variant        = hash_input.variant;
    verify_result = verify_low_level(&verify_input);
    if (verify_result.err != ARGON2_OK || verify_result.is_valid == false) {
        fprintf(
            stdout,
            "Failed to validate hash when it should have been valid. Argon2 Error: %s\n",
            argon2_error_message(verify_result.err)
        );
        exit(1);
    }

    print_encoded(hash_result.encoded);
    print_hash(hash_result.hash, hash_result.hash_len);

    free(hash_result.encoded);
    free(hash_result.hash);

    return 0;
}
