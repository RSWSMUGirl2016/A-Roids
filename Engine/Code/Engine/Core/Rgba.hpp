#pragma once
#pragma warning (disable: 201 34)


template <typename _Tx>
struct type_data
{
public:
    static _Tx  constexpr      ZERO = (_Tx)0;
    static _Tx  constexpr      ONE = (_Tx)1;
    static _Tx  constexpr      MASK = static_cast<_Tx>(-1);
    static bool constexpr      IS_SIGNED = MASK < ZERO;
    static _Tx  constexpr      SIGNBIT = IS_SIGNED ? (_Tx)(ONE << (_Tx)((sizeof(_Tx) * 8 - 1))) : ZERO;
    static _Tx  constexpr      MAX = IS_SIGNED ? (MASK & ~SIGNBIT) : MASK;
    static _Tx  constexpr      MIN = SIGNBIT;
    static _Tx  constexpr      HALFMAX = (MAX / 2) + 1;
};

template <typename _T>
inline void Swap(_T &a, _T &b)
{
    _T temp = a;
    a = b;
    b = temp;
}

inline void ByteFlip(void *arg, unsigned int size)
{
    unsigned int count = size >> 1;
    unsigned char *buffer = (unsigned char*)arg;
    for (unsigned int i = 0; i < count; i++) {
        unsigned int j = size - i - 1;
        Swap(buffer[i], buffer[j]);
    }
}

template <typename T>
inline void ByteFlip(T &v)
{
    ByteFlip(&v, sizeof(T));
}

constexpr inline bool IsLittleEndian()
{
    return (((unsigned char*)(&type_data<unsigned int>::ONE))[0] == 0x01);
}

inline void MakeBigEndian(void *buffer, unsigned int buffer_size)
{
    if (IsLittleEndian()) {
        ByteFlip(buffer, buffer_size);
    }
}

template <typename T>
inline void MakeBigEndian(T &arg)
{
    MakeBigEndian(&arg, sizeof(T));
}

//------------------------------------------------------------------------
template <typename T>
inline T ToBigEndian(T const &v)
{
    T copy = v;
    MakeBigEndian(copy);
    return copy;
}

static float Clamp01(float v)
{
    if (v < 0.0f) {
        return 0.0f;
    }
    else if (v >= 1.0f) {
        return 1.0f;
    }
    else {
        return v;
    }
}

struct rgba_u32
{
    union {
        struct {
            unsigned int r : 8;
            unsigned int g : 8;
            unsigned int b : 8;
            unsigned int a : 8;
        };
        unsigned int data;
    };

    //------------------------------------------------------------------------
    // U8 Ops
    inline void set(unsigned char pr, unsigned char pg, unsigned char pb, unsigned char pa)
    {
        data = ((unsigned int)(pr)
            | ((unsigned int)(pg) << 8)
            | ((unsigned int)(pb) << 16)
            | ((unsigned int)(pa) << 24));
    }

    inline void set(unsigned char pr, unsigned char pg, unsigned char pb)
    {
        set(pr, pg, pb, 0xff);
    }

    inline void set_red_char(unsigned char v)
    {
        data = (data & ~(0xff)) | (v);
    }

    inline void set_blue_char(unsigned char v)
    {
        data = (data & ~(0xff << 8)) | (v << 8);
    }

    inline void set_green_char(unsigned char v)
    {
        data = (data & ~(0xff << 16)) | (v << 16);
    }

    inline void set_alpha_char(unsigned char v)
    {
        data = (data & ~(0xff << 24)) | (v << 24);
    }

    inline void set_rgb_char(unsigned char pr, unsigned char pg, unsigned char pb)
    {
        unsigned int rgb = ((unsigned int)(pr)
            | ((unsigned int)(pg) << 8)
            | ((unsigned int)(pb) << 16));

        data = (data & 0xff000000) | rgb;
    }

    //------------------------------------------------------------------------
    // Floating Point Ops
    inline void set(float pr, float pg, float pb, float pa)
    {
        data = ((((unsigned int)(Clamp01(pr) * 255.0f + .5f) & 0xff))
            | (((unsigned int)(Clamp01(pg) * 255.0f + .5f) & 0xff) << 8)
            | (((unsigned int)(Clamp01(pb) * 255.0f + .5f) & 0xff) << 16)
            | (((unsigned int)(Clamp01(pa) * 255.0f + .5f) & 0xff) << 24));
    }

    inline void set(float pr, float pg, float pb)
    {
        set(pr, pg, pb, 1.0f);
    }

    inline void set_red(float v)
    {
        set_red_char((unsigned char)(v * 255.0f));
    }

    inline void set_green(float v)
    {
        set_green_char((unsigned char)(v * 255.0f));
    }

    inline void set_blue(float v)
    {
        set_blue_char((unsigned char)(v * 255.0f));
    }

    inline void set_alpha(float v)
    {
        set_alpha_char((unsigned char)(v * 255.0f));
    }

    inline void set_rgb_char(float pr, float pg, float pb)
    {
        unsigned int rgb = ((((unsigned char)(pr * 255.0f) & 0xff))
            | (((unsigned char)(pg * 255.0f) & 0xff) << 8)
            | (((unsigned char)(pb * 255.0f) & 0xff) << 16));

        data = (data & 0xff000000) | rgb;
    }

    inline void get_floats(float *rgba_out) const
    {
        rgba_out[0] = (float)(data & 0x000000ff) / 255.0f;
        rgba_out[1] = (float)((data & 0x0000ff00) >> 8) / 255.0f;
        rgba_out[2] = (float)((data & 0x00ff0000) >> 16) / 255.0f;
        rgba_out[3] = (float)((data & 0xff000000) >> 24) / 255.0f;
    }

    //////////////////////////////////////////////////////////////////////////
    // Dealing with another rgba_u32
    inline void set_rgb(rgba_u32 const &c)
    {
        data = ((data & 0xff000000) | (c.data & 0x00ffffff));
    }

    //////////////////////////////////////////////////////////////////////////
    // Constructors
    rgba_u32() {}

    rgba_u32(unsigned int v)
    {
        data = ToBigEndian(v);
    }

    rgba_u32(unsigned char r, unsigned char g, unsigned char b, unsigned char a)
    {
        set(r, g, b, a);
    }

    rgba_u32(float r, float g, float b, float a)
    {
        set(r, g, b, a);
    }

    rgba_u32(float r, float g, float b)
    {
        set(r, g, b, 1.0f);
    }
};

class Rgba 
{
public:
	static const Rgba WHITE;
    static const Rgba BLACK;
    static const Rgba YELLOW;
    static const Rgba GREEN;
    static const Rgba RED;
    static const Rgba BLUE;
    static const Rgba MAGENTA;
    static const Rgba CYAN;
    static const Rgba MEDIUM_GREY;
    static const Rgba LIGHT_BLUE;
    static const Rgba PURPLE;
    static const Rgba ORANGE;
    static const Rgba TRANSPRNT;
	unsigned char r;
	unsigned char g;
	unsigned char b;
	unsigned char a;
public:
	Rgba();
	~Rgba();
	explicit Rgba(unsigned char redByte, unsigned char greenByte,
		unsigned char blueByte, unsigned char alphaByte = 255);
	//explicit Rgba(float normalizedRed, float normalizedGreen,
		//float normalizedBlue, float normalizedAlpha = 1.0f);
public:
	void SetAsBytes(unsigned char redByte, unsigned char greenByte,
		unsigned char blueByte, unsigned char alphaByte = 255);
	void SetAsFloats(float normalizedRed, float normalizedGreen,
		float normalizedBlue, float normalizedAlpha = 1.0f);
	void GetAsFloats(float& out_normalizedRed, float& out_normalizedGreen, float& out_normalizedBlue, float& out_normalizedAlpha) const;
	void ScaleRGB(float rgbScale);
	void ScaleAlpha(float alphaScale);
	void SubtractRGB(float rgbSubtract);
	void SubtractAlpha(float alphaSubtract);
	void AddAlpha(float alphaAdd);
	unsigned char ConvertFloatColorToByteColor(float normalizedColor);
	float Normalize(unsigned char colorByte);
public:
	bool operator==(const Rgba& rgbaToEqual) const;
	bool operator!=(const Rgba& rgbaToNotEqual) const;
};