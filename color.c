#include "types.h"



Color color_add(Color c1, Color c2)
{
    return (Color) { c1.r + c2.r, c1.g + c2.g, c1.b + c2.b };
}

Color color_add_scalar(Color a, float scalar)
{
    return (Color) { a.r + scalar, a.g + scalar, a.b + scalar };
}

Color color_sub(Color c1, Color c2)
{
    return (Color) { c1.r - c2.r, c1.g - c2.g, c1.b - c2.b };
}

Color color_mul(Color c1, Color c2)
{
    return (Color) { c1.r * c2.r, c1.g * c2.g, c1.b * c2.b };
}

Color color_mul_scalar(Color c, double k)
{
    return (Color) { c.r * k, c.g * k, c.b * k };
}
