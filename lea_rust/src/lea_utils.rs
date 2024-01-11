pub fn rotl32(val: u32, shift: u32) -> u32 {
    val.rotate_left(shift)
}

pub fn rotr32(val: u32, shift: u32) -> u32 {
    val.rotate_right(shift)
}

pub fn reverse_byte_order(word: u32) -> u32 {
    word.swap_bytes()
}

pub fn string_to_word_array(hex_string: &str) -> Vec<u32> {
    hex_string
        .as_bytes() // Convert the string slice to byte slice
        .chunks(8)   // Process 8 characters at a time
        .map(|chunk| {
            let hex_chunk = std::str::from_utf8(chunk) // Convert each chunk to a str
                .unwrap_or_else(|_| panic!("Invalid UTF-8"));
            u32::from_str_radix(hex_chunk, 16) // Parse the hex string as u32
                .unwrap_or_else(|_| panic!("Invalid hex format"))
        })
        .collect() // Collect the parsed values into a Vec<u32>
}

// Example usage:
// let word_array = string_to_word_array("deadbeef...");


pub fn print_big_endian(array: &[u32]) {
    for (i, &value) in array.iter().enumerate() {
        if i == 4 {
            println!();
        }
        for j in (0..4).rev() {
            print!("{:02x}:", (value >> (j * 8)) & 0xFF);
        }
    }
    println!();
}

pub fn print_little_endian(array: &[u32]) {
    println!("(gdb) x/--xb ---");
    for (i, &value) in array.iter().enumerate() {
        if i % 2 == 0 {
            print!("\n0x7fffffffXXXX: ");
        }
        for j in 0..4 {
            print!("{:02x}:", (value >> (j * 8)) & 0xFF);
        }
    }
    println!();
}

pub fn print_enc_round_keys(enc_round_key: &[u32]) {
    println!("\nEncryption RoundKey: ");
    let mut j = 0;
    for (i, &key) in enc_round_key.iter().enumerate() {
        if i % 6 == 0 {
            println!("\nEnc_Round[{:02}] | ", j);
            j += 1;
        }
        print!("{:08x}:", key);
    }
    println!();
}


