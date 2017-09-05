#version 430

layout(location = 0) out vec4 giColor;

in vec2 uv;

uniform sampler2D pMap;
uniform sampler2D nMap;
uniform sampler2D vplPMap;
uniform sampler2D vplAMap;
uniform sampler2D vplNMap;
uniform mat4 smMat;

const int lightAtten = 5;

void main ()
{
	vec3 wsP = texture2D(pMap, uv).xyz;
	vec3 gi = vec3(0.0f, 0.0f, 0.0f);
	for (int i = 1; i < 10; i++)
	{
		for (int j = 1; j < 10; j++)
		{
			vec2 vplUv = vec2(i * 0.1f, j * 0.1f);
			vec3 vplPos = texture2D(vplPMap, vplUv).xyz;
			float d = distance(wsP, vplPos);
			if (d > 0.0f && d <= 5.0f)
			{
				vec3 v = normalize(wsP - vplPos);
				vec3 vplN = texture2D(vplNMap, vplUv).xyz;
				//if (dot(v, vplN) >= 0.0f)
				{
					vec3 N = texture2D(nMap, uv).xyz;
					vec3 vplColor = texture2D(vplAMap, vplUv).xyz;
					gi += max(dot(N, -v), 0.0f) * vplColor * (lightAtten - d)/lightAtten;
				}
			}
		}
	}

	giColor = vec4(gi, 1.0f);
}