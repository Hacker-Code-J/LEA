mod lea_utils;

use crate::lea_config::{KEY_SIZE, Nr, TOTAL_RK};

const delta: [u32; 8] = [
    0xc3efe9dbu32, 0x44626b02u32, 0x79e27c8au32, 0x78df30ecu32,
    0x715ea49eu32, 0xc785da0au32, 0xe04ef22au32, 0xe5c40957u32
];

pub fn lea_enc_key_schedule(round_keys: u32, key: u32) {
    let mut t: Vec<u32> = vec![0; 4];
    for i in 0..4 {
        t[i] = reverse_byte_order(key[i]);
    }

    for i in 0..Nr as usize {
        t[0] = rotl32(t[0].wrapping_add(rotl32(delta[i % 4], i as u32 + 0)), 1);
        t[1] = rotl32(t[1].wrapping_add(rotl32(delta[i % 4], i as u32 + 1)), 3);
        t[2] = rotl32(t[2].wrapping_add(rotl32(delta[i % 4], i as u32 + 2)), 6);
        t[3] = rotl32(t[3].wrapping_add(rotl32(delta[i % 4], i as u32 + 3)), 11);
    
        round_keys[i * 6 + 0] = t[0];
        round_keys[i * 6 + 1] = t[1];
        round_keys[i * 6 + 2] = t[2];
        round_keys[i * 6 + 3] = t[1];
        round_keys[i * 6 + 4] = t[3];
        round_keys[i * 6 + 5] = t[1];
    }
}