#include <iostream>

#include <cgmath>
#include <cgasset>
#include <cgtexture>


////////////////////////////////////////////////////////////////////////////////
struct Surface
{
	float t;
	vec3 n;
	vec2 uv;
	vec2 uvdx;
	vec2 uvdy;
};
static const Surface sNone = {-1,{0,0,0},{0,0},{0,0},{0,0}};
////////////////////////////////////////////////////////////////////////////////
const Surface isPlane(const vec3& ro,const vec3& rd,const vec3 rddx,const vec3 rddy,const vec3& n,const float d)
{
	float t = dot(n,rd);
	if(abs(t) < 0) return sNone;
    t = dot(n,d*rd-ro)/t;
    vec3 pos = ro+t*rd;
    float tdx = dot(n,d*rddx-ro)/dot(n,rddx);
    float tdy = dot(n,d*rddy-ro)/dot(n,rddy);
    vec3 posdx = ro+tdx*rddx - pos;
    vec3 posdy = ro+tdy*rddy - pos;

	return {t,n,{pos.x,-pos.z},{posdx.x,-posdx.z},{posdy.x,-posdy.z}};
}
////////////////////////////////////////////////////////////////////////////////
const Surface opU(const Surface s1,const Surface s2)
{
    if((s2.t < s1.t && s2.t > 0) || s1.t < 0) return s2;
    return s1;
}
////////////////////////////////////////////////////////////////////////////////
const Surface castRay(const vec3& ro,const vec3& rd,const vec3& rddx,const vec3& rddy)
{
	Surface s = sNone;
	s = isPlane(ro,rd,rddx,rddy,vec3(0,1,0),0);
	return s;
}
////////////////////////////////////////////////////////////////////////////////
vec3 renderScene(const vec3& ro,const vec3& rd,const vec3& rddx,const vec3& rddy,auto& texture)
{
	vec3 color(0);
	const Surface s = castRay(ro,rd,rddx,rddy);

	if(s.t < 0) return color;

    rgb32f cne = nearest<Wrap::REPEAT>(texture,s.uv.x*texture.w,s.uv.y*texture.h,0);
    rgb32f cbi = bilinear<Wrap::REPEAT>(texture,s.uv.x,s.uv.y,log2(0.5e-3f+0.03f*s.t/dot(s.n,-rd)));
    rgb32f cti = trilinear<Wrap::REPEAT>(texture,s.uv.x,s.uv.y,log2(0.5e-3f+0.03f*s.t/dot(s.n,-rd)));
    rgb32f can = anisotropic<Wrap::REPEAT>(texture,s.uv.x,s.uv.y,s.uvdx.x,s.uvdx.y,s.uvdy.x,s.uvdy.y);

	if(abs(rd.x-0.45) < 5e-3 || abs(rd.x+0.45) < 5e-3)
		return color;

	if(rd.x < -0.45) return cti;
	if(rd.x > 0.45) return can;
	return cne;
}
////////////////////////////////////////////////////////////////////////////////
int main(int argc,char* argv[])
{
	Bitmap bitmap("cat.png",Bitmap::RGB32F);

	TexLod_rgb32f texture(bitmap);
	TexLod_rgb32f::maxAniso = 32;

	bitmap = Bitmap(1024,1024,Bitmap::RGB32F);

	float w = bitmap.width;
	float h = bitmap.height;
	const vec3 ro = vec3(0,2,-2);
	const vec3 rd = normalize(vec3(0,-1,1));

	const mat4 mvp = proj(w/h,90.0f,0.1)*lookAt(ro,ro+rd);
	const mat3 invMVP = inverse(mat3(mvp));

	int spp = 1,sqrtspp = sqrt(spp);
	for(float i=0; i<w; ++i)
		for(float j=0; j<h; ++j)
		{
			vec3 color(0);
			for(int k=0; k<spp; ++k)
			{
				const vec2 windowSize(w,h);
				const vec2 windowCoords(i,j);
				const vec2 screenOffset = vec2(mod(k,sqrtspp),k/sqrtspp)/sqrtspp;
				const vec2 screenCoords = 
					2*(windowCoords+0.5 + screenOffset)/windowSize - 1;

    			const vec3 rd = normalize(invMVP*vec3(screenCoords,1));
    			const vec3 rddx = normalize(invMVP*vec3(screenCoords+vec2(1.0f/w,0),1));
    			const vec3 rddy = normalize(invMVP*vec3(screenCoords+vec2(0,1.0f/h),1));

				color += renderScene(ro,rd,rddx,rddy,texture);
			}
			color /= spp;
			color = pow(color,vec3(0.4545));
			bitmap(*(const rgb32f*)&color,i,h-j-1);
		}

	bitmap.write("out.png",Bitmap::RGB8);

	return 0;
}
////////////////////////////////////////////////////////////////////////////////
