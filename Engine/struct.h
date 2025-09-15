#pragma once


struct Vtx
{
	Vec3	vPos;
	Vec2	vUV;
	Vec4	vColor;
};


struct tDebugShapeInfo
{
	DEBUG_SHAPE Shape;

	Vec3		WorldPos;
	Vec3		Scale;
	Vec3		Rotation;
	Matrix		matWorld;

	Vec4		Color;
	float		Duration;	// DebugShape ���� �ð�
	float		Time;		// ���� ���� �ð�

	bool		DepthTest;	// ���̰˻縦 ����
};

struct tTask
{
	TASK_TYPE	Type;
	DWORD_PTR	Param0;
	DWORD_PTR	Param1;
	DWORD_PTR	Param2;
};

struct tLight2DInfo
{
	Vec3		vColor;		// ���� ����	
	Vec3		vWorldPos;	// ���� ��ġ
	Vec3		vDir;		// ���� ���� ����
	int			Type;		// ���� Ÿ��(LIGHT_TYPE)
	float		Radius;		// ���� �����ݰ�
	float		Angle;		// ���� ��������	

	float		InnerRadius;
	float		spotDirection;
	Vec2		padding;
    
};

// Particle
struct tParticle
{
	Vec4	vColor;
	Vec3	vLocalPos;
	Vec3	vWorldPos;
	Vec3	vWorldInitScale;
	Vec3	vWorldCurrentScale;
	Vec3	vWorldRotation;		// ���� ȸ���� �־���
	Vec3	vRotationVelocity;



	Vec3	vForce;				// Particle �� ������ ��
	Vec3	vVelocity;

	float	NoiseForceAccTime;	// Noise Force �ҿ� �����ϴ� ���� �ð�
	Vec3	NoiseForceDir;		// Noise Force �� ����

	float	Mass;
	float	Life;
	float	Age;
	float	NormalizedAge;
	int		Active;

	Vec3	Padding;
};

// Particle Module
struct tParticleModule
{
	// Spawn
	UINT	SpawnRate;				// �ʴ� ��ƼŬ �������� (Spawn Per Second)
	Vec4	vSpawnColor;			// ���� ���� ����
	Vec4	vSpawnMinScale;			// ���� �� �ּ� ũ��
	Vec4	vSpawnMaxScale;			// ���� �� �ִ� ũ��

	float	MinLife;				// �ּ� ����
	float	MaxLife;				// �ִ� ����

	UINT	SpawnShape;				// 0 : Box,  1: Sphere
	Vec3	SpawnShapeScale;		// SpawnShapeScale.x == Radius

	UINT	BlockSpawnShape;		// 0 : Box,  1: Sphere
	Vec3	BlockSpawnShapeScale;	// SpawnShapeScale.x == Radius

	UINT	SpaceType;				// 0 : LocalSpace, 1 : WorldSpace

	// Spawn Burst
	UINT	SpawnBurstCount;		// �ѹ��� �߻���Ű�� Particle ��
	UINT	SpawnBurstRepeat;
	float	SpawnBurstRepeatTime;

	// Add Velocity
	UINT	AddVelocityType;		// 0 : Random, 1 : FromCenter, 2 : ToCenter, 4 : Fixed 
	Vec3	AddVelocityFixedDir;
	float	AddMinSpeed;
	float	AddMaxSpeed;

	// Scale Module
	UINT	ScaleType;		// 0 : �ܹ��� (�ñ⸸ �ϰų� �ٱ⸸ �ϰų�) 1 : ����� �߰� ���ڰ� ���� ���� (�þ��� �پ���.)
	float	ScaleX;
	float	ScaleY;


	float	StartScale;
	float	EndScale;

	// Drag Module
	float	DestNormalizedAge;
	float	LimitSpeed;

	// Noise Force Module
	float	NoiseForceTerm;		// Noise Force �����Ű�� ��
	float	NoiseForceScale;	// Noise Force ũ��

	// Render Module
	Vec3	EndColor;			// ���� ����
	UINT	FadeOut;			// 0 : Off, 1 : Normalized Age
	float	FadeOutStartRatio;	// FadeOut ȿ���� ���۵Ǵ� Normalized Age ����
	UINT	VelocityAlignment;	// �ӵ� ���� 0 : Off, 1 : On
	UINT	UseCrossMesh;

	// Rotation Module (�߰�)
	UINT    RotationType;       // 0: Random, 1: Fixed
	Vec3    StartMinRotation;   // ���� �ּ� ȸ��
	Vec3    StartMaxRotation;   // ���� �ִ� ȸ��
	Vec3    MinRotationSpeed;   // �ּ� ȸ�� �ӵ�
	Vec3    MaxRotationSpeed;   // �ִ� ȸ�� �ӵ�
	float   RotationDamping;    // ȸ�� ������
	UINT    RotationSpace;      // 0: Local, 1: World

	// Module On / Off
	int		Module[(UINT)PARTICLE_MODULE::END];
};



// ====================
// ��� ���� ���� ����ü
// ====================
struct TransInfo
{
	Matrix	matWorld;
	Matrix	matView;
	Matrix	matProj;

	Matrix	matWV;
	Matrix	matWVP;
	float	objectSpawnTime;
	Vec3	padding;
};
extern TransInfo g_Trans;

struct MtrlConst
{
	int		iArr[4];
	float	fArr[4];
	Vec2	v2Arr[4];
	Vec4	v4Arr[4];
	Matrix	mat[2];
	int		bTex[TEX_END];
};

struct SpriteInfo
{
	Vec2 LeftTopUV;
	Vec2 SliceUV;
	Vec2 BackgroundUV;
	Vec2 OffsetUV;
	int  SpriteUse;
	
	Vec3	HitColor;
	float HitEffectRatio;   // ��Ʈ ȿ�� ���� (0~1)
	int		UseHitEffect;
	Vec2	padding;

};

struct GlobalData
{
	Vec2	RenderResolution;	// ������ �ػ�

	float	DeltaTime;		// DT
	float   Time;			// �����ð�

	float	DT_Engine;		// DT
	float   Time_Engine;	// �����ð�

	int		Light2DCount;		// 2D ���� ����
	int		Light3DCount;		// 3D ���� ����

};

// Pixel
struct tPixel
{
	BYTE b;
	BYTE g;
	BYTE r;
	BYTE a;
};


struct tStageData
{
	int Idx;
	wstring StageName;
	wstring StageBGM;


};

struct tPlayerData
{
	int     Cur_Hp;
	int     Max_Hp;
	int     Cur_Mp;
	int     Max_Mp;

	int     Atk;
	int     SlayCount;

};



extern GlobalData g_Data;

