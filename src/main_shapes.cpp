#include <iostream>

#include <cgmath>
#include <cgasset>
#include <cgtexture>
#include <cgtrace>


////////////////////////////////////////////////////////////////////////////////
template<typename HIT>
const HIT opU(const HIT& h1,const HIT& h2)
{
    if((h2.t < h1.t && h2.t > 0) || h1.t < 0) return h2;
    return h1;
}
////////////////////////////////////////////////////////////////////////////////
int main(int argc,char* argv[])
{
	const int nVerts = 42;
	const vec3 mesh[] = {
	vec3(-1.468689,-1.963555,0.097573), vec3(0.505874,-0.205919,-0.007964),
	vec3(-1.517807,-0.995600,1.232863), vec3(-2.227164,-1.342829,0.557186),
	vec3(-0.766356,-1.047875,-0.828205), vec3(0.845819,-0.518346,-1.008734),
	vec3(0.873975,1.087834,0.681610), vec3(-0.845819,0.518346,1.008734),
	vec3(-0.686115,0.545674,0.000643), vec3(1.249599,1.088200,-1.412118),
	vec3(2.227164,1.342829,-0.557186), vec3(1.468689,1.963555,-0.097573),
	vec3(-1.755414,-1.739347,0.782005), vec3(-0.194866,-0.862768,0.356910),
	vec3(-0.667961,-0.870884,0.880459), vec3(0.721339,-0.590360,-0.437116),
	vec3(-0.366120,-1.458826,-0.535563), vec3(-1.019405,-1.153469,0.670362),
	vec3(-2.201241,-1.374495,1.052163), vec3(-1.313726,-1.770073,-0.429451),
	vec3(-1.759552,-1.405222,-0.159297), vec3(-0.032841,-1.079332,-0.710543),
	vec3(1.415622,0.778577,0.041415), vec3(1.806260,0.484624,-0.920424),
	vec3(-0.048879,0.238348,0.695525), vec3(0.674638,0.330241,0.642611),
	vec3(-1.704303,-0.122680,0.781835), vec3(-1.389311,-0.280524,1.317585),
	vec3(-0.192113,0.066173,-0.456593), vec3(-1.533283,-0.503614,0.171707),
	vec3(1.389311,0.280524,-1.317585), vec3(0.441692,-0.030727,-1.211468),
	vec3(1.759552,1.405222,0.159297), vec3(-0.046708,0.920600,1.079724),
	vec3(-0.713616,0.121668,0.470251), vec3(0.667961,0.870884,-0.880459),
	vec3(2.201241,1.374495,-1.052163), vec3(1.313726,1.770073,0.429451),
	vec3(1.753655,1.035890,-0.115090), vec3(0.366120,1.458826,0.535563),
	vec3(0.366350,1.542033,0.093008), vec3(1.755414,1.739347,-0.782006),
	};

	Sphere s[nVerts];
	for(int i=0; i<nVerts; ++i)
	{
		s[i].o = mesh[i];
		s[i].r = 0.05;
	}
	OBB obb(mesh,nVerts);
	AABB aabb(obb);
	Sphere sphere(aabb);


	Bitmap bitmap(1024,1024,Bitmap::RGB32F);

	//TexLod_rgb32f texture(Bitmap("res/earth.png",Bitmap::RGB32F));

	vec3 ro(0,0,3);
	vec3 rd = normalize(-ro);
	Camera camera(bitmap.width,bitmap.height,M_PI*0.3,ro,rd);

	int n = 9;
	for(int k=0; k<n; ++k)
	{
		ro = vec3(sin(2*M_PI*float(k)/n),1,cos(2*M_PI*float(k)/n))*8;
		rd = normalize(-ro);
		camera = Camera(bitmap.width,bitmap.height,M_PI*0.3,ro,rd);

		for(int i=0; i<bitmap.width; ++i)
			for(int j=0; j<bitmap.height; ++j)
			{
				vec3 color(0);

				Rayd ray = camera.createRay<Rayd>(i,j);

				Hitn hitPoints;
				hitPoints.t = -1;
				for(int k=0; k<nVerts; ++k)
					hitPoints = opU(hitPoints,intersect<Hitn>(ray,s[k]));

				Hituv hitAABB = intersect<Hituv>(ray,aabb);
				Hituv hitOBB = intersect<Hituv>(ray,obb);
				Hituv hitSphere = intersect<Hituv>(ray,sphere);

				if(hitPoints.t >= 0)
					color = (0.2+0.8*clamp(dot(normalize(vec3(1,2,3)),hitPoints.n),0,1));

				if(hitAABB.t >= 0)
				{
					float t = step(0.49,max(abs(hitAABB.uv.x-0.5),abs(hitAABB.uv.y-0.5)));
					if(t > 0) color = vec3(1,0,0);
				}
				if(hitOBB.t >= 0)
				{
					float t = step(0.49,max(abs(hitOBB.uv.x-0.5),abs(hitOBB.uv.y-0.5)));
					if(t > 0) color = vec3(0,1,0);
				}
				if(hitSphere.t >= 0)
				{
					float t = step(0.90,1-clamp(dot(-ray.d,hitSphere.n),0,1));
					if(t > 0) color = vec3(0,0,1);
				}

				bitmap(&color,i,j);
			}

		std::stringstream ss;
		ss << "out-" << k << ".png";
		bitmap.write(ss.str().c_str(),Bitmap::RGB8);
	}

	return 0;
}
