#include <iostream>

#include <cgmath>
#include <cgasset>
#include <cgtexture>
#include <cgtrace>

#include "Mesh.h"


////////////////////////////////////////////////////////////////////////////////
float bvhSAH(const auto& bvh)
{
	float sah = 0;
	for(const auto&node: bvh.nodes)
		sah += node.volume.SAH();
	return sah;
}
////////////////////////////////////////////////////////////////////////////////
int main(int argc,char* argv[])
{
	const Scene scene("res/cornellbox.obj");
	
	const VertStream vsPositions(scene,(const int[]){ VertStream::COORDS,3,0});

	TriangleIndicesArray primLayout;
	primLayout.data     = (vec3*)&vsPositions.vertexArray[0];
	primLayout.indices  = ((ivec3*)&vsPositions.indices[0]);
	primLayout.backface = true;
	primLayout.count    = uint(vsPositions.indices.size()/3);

	BVH<OBB,4> bvh = BVH<OBB,4>(BVH<KDOP<4>,4>(primLayout));

	Bitmap bitmap(1024,1024,Bitmap::RGB32F);

	vec3 ro(0,0,5);
	vec3 rd = normalize(-ro);
	Camera camera(bitmap.width,bitmap.height,28.1*M_PI/180.0f,ro,rd);

	Compute compute(bitmap.width,bitmap.height);
	compute.dispatch([&](const ComputeInput<2>& input)
	{
		const ivec2 coords(input.id[0],input.id[1]);
		const Ray ray = camera.createRay<Ray>(coords.x,coords.y);

		static const vec3 light(0,0.5,0);

		vec3 color(0);

		Hitn hitSurface = traceClosestHit<Hitn>(ray,bvh,primLayout);

		if(hitSurface.t >= 0)
		{
			vec3 p = ray.o+hitSurface.t*ray.d+hitSurface.n*5e-5;
			vec3 l = light-p;
			Ray rayLight;
			rayLight.o = p;
			rayLight.d = l;
			Hitf hitLight = traceClosestHit<Hitf>(rayLight,bvh,primLayout);

			hitSurface.n = normalize(hitSurface.n);
			l = normalize(l);

			float lightVisibility = !(hitLight.t >= 0 && hitLight.t <= 1);

			vec3 dif = 0.5;
			vec3 amb = 0.5;

			float ao = 1;
			for(uint i=0; i<32; ++i)
			{
				vec3 d = hemisphereSample(cosineDistribution(genSample2()),hitSurface.n);
				Ray rayAO;
				rayAO.o = p;
				rayAO.d = d;
				Hitf hitAO = traceClosestHit<Hitf>(rayAO,bvh,primLayout);
				ao += clamp(1-exp(-hitAO.t),0,1);
			}
			ao /= 32;

			color = ao*amb+lightVisibility*dif*clamp(dot(l,hitSurface.n),0,1);
		}

		bitmap(&color,coords.x,coords.y);
	});
	compute.sync();

	bitmap.write("out.png",Bitmap::RGB8);

	return 0;
}
