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
					int pitch = SeedLenght >> o;
					int sample1 = ((int)x / pitch) * pitch % (int)SeedLenght;
					int sample2 = (sample1 + pitch) % (int)SeedLenght;

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

	template<size_t SeedSize, size_t NoiseWidth, size_t NoiseHeight>
	class Perlin2D
	{
	public:
		Perlin2D()
		{
			size_t len = SeedSize * SeedSize;
			for (size_t i = 0; i < len; i++)
				m_Seed[i] = Utility::Random::Float(1.0f);
		}

		const float* GenerateNoise(size_t octaves)
		{
			for (size_t y = 0; y < NoiseHeight; y++)
			{
				for (size_t x = 0; x < NoiseWidth; x++)
				{
					float noise = 0.0f;
					float scale = 1.0f;
					float scaleAcc = 1.0f;
					for (size_t o = 0; o < octaves; o++)
					{
						int pitch = SeedSize >> o;

						int sampleX1 = (((int)x / pitch) * pitch) % SeedSize;
						int sampleY1 = (((int)y / pitch) * pitch) % SeedSize;

						int sampleX2 = (sampleX1 + pitch) % (int)SeedSize;
						int sampleY2 = (sampleY1 + pitch) % (int)SeedSize;

						float blendX = (float)(x - sampleX1) / (float)pitch;
						float blendY = (float)(y - sampleY1) / (float)pitch;

						float sampleT = (1.0f - blendX) * m_Seed[sampleY1 * SeedSize + sampleX1] + blendX * m_Seed[sampleY1 * SeedSize + sampleX2];
						float sampleB = (1.0f - blendX) * m_Seed[sampleY2 * SeedSize + sampleX1] + blendX * m_Seed[sampleY2 * SeedSize + sampleX2];

						noise += (blendY * (sampleB - sampleT) + sampleT) * scale;
						scaleAcc += scale;
						scale /= 2.0f;
					}
					m_Noise[y * (size_t)NoiseWidth + x] = noise / scaleAcc;
				}
			}
			return m_Noise;
		}

		float GetSeedLength() const { return SeedSize; }
		float GetNoiseWidth() const { return NoiseWidth; }
		float GetNoiseHeight() const { return NoiseHeight; }

		const float* GetSeed() const { return m_Seed; }
		float* GetSeed() { return m_Seed; }

		const float* GetNoise() const { return m_Noise; }
		float* GetNoise() { return m_Noise; }

	private:
		float m_Seed[SeedSize * SeedSize];
		float m_Noise[NoiseWidth * NoiseHeight];
	};
}