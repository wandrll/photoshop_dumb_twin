#include "utils.hpp"



double  hue2rgb(float p, float q, float t){
            if(t < 0) t += 1;
            if(t > 1) t -= 1;
            if(6 * t < 1) return p + (q - p) * 6 * t;
            if(2 * t < 1) return q;
            if(3 * t < 2) return p + (q - p) * (2./3. - t) * 6;
            return p;
        }


Color convert_color(float h, float s, float l, int opacity){
    float R = 0;
    float G = 0;
    float B = 0;

    if(s == 0){
        R = G = B = l; // achromatic
    }else{
        

        float q = l < 0.5 ? l * (1 + s) : l + s - l * s;
        float p = 2 * l - q;
        R = hue2rgb(p, q, ((float)h) + 1./3.);
        G = hue2rgb(p, q, ((float)h));
        B = hue2rgb(p, q, ((float)h) - 1./3.);
    }

    Color result(R*255, G*255, B*255);
    result.color.a = opacity;
    return result;
}

