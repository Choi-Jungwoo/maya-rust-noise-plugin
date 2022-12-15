pub mod noise_vector;

#[no_mangle]
pub extern "C" fn new_rust_noise_vector() -> noise_vector::NoiseVector {
    noise_vector::NoiseVector::new()
}

#[no_mangle]
pub extern "C" fn gen_noise(noise_vector: &mut noise_vector::NoiseVector, displacement: f32) {
    noise_vector.gen(displacement);
}
