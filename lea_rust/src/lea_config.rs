pub const LEA_VERSION: u32 = 128; // 128 / 192 / 256


// Using a match statement to determine KEY_SIZE, Nr, and TOTAL_RK
// based on LEA_VERSION. This is more idiomatic in Rust compared to
// if-else chains.
pub const KEY_SIZE: u32 = match LEA_VERSION {
    192 => 6,
    256 => 8,
    _   => 4,
};
pub const Nr: u32 = match LEA_VERSION {
    192 => 28,
    256 => 32,
    _   => 24,
};
pub const TOTAL_RK: u32 = match LEA_VERSION {
    192 => 168,
    256 => 192,
    _   => 144,
};