mod lea_config;
mod lea_utils;
mod lea;

use crate::lea_config::{KEY_SIZE, Nr, TOTAL_RK};


fn main() {
    // In Rust, arrays need to know their size at compile time.
    // KEY_SIZE must be a constant known at compile time.
    let mut key = [0u32; KEY_SIZE];

    // Rust string literals are &str, a slice pointing to a UTF-8 sequence.
    let key_string = "0f1e2d3c4b5a69788796a5b4c3d2e1f0";
    // Other key strings for different key sizes can be defined similarly.

    // Convert the string to the word array. Rust doesn't pass arrays around, 
    // it either uses slices (&[T]) or Vec<T> for dynamic arrays.
    key.copy_from_slice(&string_to_word_array(key_string)[..KEY_SIZE]);

    // Printing in Rust uses println! and print! macros.
    println!("Key: ");
    print_big_endian(&key);

    // To use print_little_endian instead, uncomment the following line:
    // print_little_endian(&key);

    // Rust uses Vec for dynamic-sized arrays, here for encryption round keys.
    let mut enc_roundkey = vec![0u32; TOTAL_RK];

    // Assuming leaEncKeySchedule is a function that takes slices.
    lea_enc_key_schedule(&mut enc_roundkey, &key);

    // Print encryption round keys.
    print_enc_round_keys(&enc_roundkey);
}
