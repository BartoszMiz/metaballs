#version 330
const int BALL_COUNT = 20;
const float strength = 0.25;

in vec2 fragTexCoord;

out vec4 finalColor;

uniform vec2 windowSize;
uniform vec3 metaballData[BALL_COUNT]; // XY - position z - radius

// Thanks ChatGPT <3
vec3 HSLToRGB(vec3 hsl)
{
	float hue = hsl.x;
	float saturation = hsl.y;
	float lightness = hsl.z;

	float c = (1.0 - abs(2.0 * lightness - 1.0)) * saturation;
	float x = c * (1.0 - abs(mod(hue * 6.0, 2.0) - 1.0));
	float m = lightness - 0.5 * c;

	vec3 rgb;
	if (hue < 1.0 / 6.0)
	{
		rgb = vec3(c, x, 0.0);
	}
	else if (hue < 2.0 / 6.0)
	{
		rgb = vec3(x, c, 0.0);
	}
	else if (hue < 3.0 / 6.0)
	{
		rgb = vec3(0.0, c, x);
	}
	else if (hue < 4.0 / 6.0)
	{
		rgb = vec3(0.0, x, c);
	}
	else if (hue < 5.0 / 6.0)
	{
		rgb = vec3(x, 0.0, c);
	}
	else
	{
		rgb = vec3(c, 0.0, x);
	}

	return rgb + vec3(m);
}

void main()
{
	vec2 pixelCoord = fragTexCoord * windowSize;

	float brightness = 0.0;
	for (int i = 0; i < BALL_COUNT; i++)
	{
		float value = length(pixelCoord - metaballData[i].xy);
		brightness += metaballData[i].z / value * strength;
	}

	// if (brightness <= 0.85)
	// {
	// 	finalColor = vec4(0);
	// 	return;
	// }
	brightness = clamp(brightness, 0.0, 1.0);
	vec3 color = HSLToRGB(vec3(brightness, 1.0, 0.5));
	finalColor = vec4(color.xyz, 1.0);	
}
