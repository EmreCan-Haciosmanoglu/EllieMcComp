#pragma once

class SingleBrain;
class Puppet
{
public:
	Puppet(SingleBrain* parent, int dimensions);
	~Puppet();

	void Update();
	void Draw();
	void Play(int direction);

	int GetPoints();
	inline int GetDimensions() const { return m_Dimensions; }
	inline bool IsDeath() const { return b_IsDeath; }

private:
	void InitState();
	void AddCard();
	bool MoveExist();
	bool SpaceExist();
	float* GetCopyState();
	int Sum(int* state, int size);
	int Sum(float* state, int size);
	int Think();
private:

	int* m_State;
	int m_Dimensions;
	int m_InputSize;

	bool b_IsDeath = false;

	SingleBrain* m_Parent;
};