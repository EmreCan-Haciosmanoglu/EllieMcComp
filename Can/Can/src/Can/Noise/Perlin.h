#pragma once
#include "Can/Random.h"

namespace Can::Noise
{
	template<size_t SeedLenght, size_t NoiseLenght>
	class Perlin1D
	{
	public:
		Perlin1D()
		{
			for (size_t i = 0; i < SeedLenght; i++) m_Seed[i] = Utility::Random::Float(1.0f);
		}

		const float* GenerateNoise(size_t octaves)
		{
			for (size_t x = 0; x < NoiseLenght; x++)
			{
				float noise = 0.0f;
				float scale = 1.0f;
				float scaleAcc = 0.0f;
				for (size_t o = 0; o < octaves; o++)
				{
					int pitch = NoiseLenght >> o;
					int sample1 = ((int)x / pitch) * pitch;
					int sample2 = (sample1 + pitch) % (int)NoiseLenght;

					float blend = (float)(x - sample1) / (float)pitch;
					float sample = (1.0f - blend) * m_Seed[sample1] + blend * m_Seed[sample2];

					noise += sample * scale;
					scaleAcc += scale;
					scale /= 2.0f;
				}
				m_Noise[x] = noise / scaleAcc;
			}
			return m_Noise;
		}

		float GetSeedLength() const { return SeedLenght; }
		float GetNoiseLength() const { return NoiseLenght; }

		const float* GetSeed() const { return m_Seed; }
		float* GetSeed() { return m_Seed; }

		const float* GetNoise() const { return m_Noise; }
		float* GetNoise() { return m_Noise; }

	private:
		float m_Seed[SeedLenght];
		float m_Noise[NoiseLenght];
	};
}