use rand::Rng;

/// cbindgen:derive-eq
#[repr(C)]
#[derive(Debug, Clone, Copy)]
pub struct NoiseVector {
    x: f32,
    y: f32,
    z: f32,
}

impl NoiseVector {
    pub fn new() -> NoiseVector {
        NoiseVector {
            x: 0f32,
            y: 0f32,
            z: 0f32,
        }
    }

    fn gen_point(displacement: f32) -> f32 {
        rand::thread_rng().gen::<f32>() * displacement
    }

    pub fn gen(&mut self, displacement: f32) {
        self.x = NoiseVector::gen_point(displacement);
        self.y = NoiseVector::gen_point(displacement);
        self.z = NoiseVector::gen_point(displacement);
    }
}

#[cfg(test)]
mod tests {
    use super::*;

    #[test]
    fn test_gen_noise() {
        let mut noise = NoiseVector::new();
        noise.gen(0.002);

        println!("{:?}", noise);
    }
}
