#include "BirdfulG.h"

int Width = 700;
int Height = 500;
using namespace bird;
struct vec2 {
	float x, y;
};

vec2 pillar1 = { 400,100 };
vec2 pillar2 = { 200,300 };
vec2 pillar3 = { 600,300 };

float px = 400;
float py = 400;

float dist1 = 0;
float dist2 = 0;
float dist3 = 0;
float dist(float x1, float y1, float x2, float y2) {
	return sqrtf((x2-x1)* (x2 - x1)+(y2-y1)* (y2 - y1));
}
float dist(vec2 p1, vec2 p2) {
	return sqrtf((p2.x - p1.x) * (p2.x - p1.x) + (p2.y - p1.y) * (p2.y - p1.y));
}
bool intersect(vec2 p1, vec2 p2, float r1, float r2, vec2* out1, vec2* out2) { // Returns if there is a valid intersection
	float d12 = dist(p1, p2);
	float tr1 = r1;
	float tr2 = r2;
	if ( d12>(r1 + r2)) {
		
		tr1 += (d12-(r1+r2));
		tr2 += (d12-(r1+r2));
	}

	float d1 = acosf(((tr1 * tr1) + (d12 * d12) - (tr2 * tr2)) / (2.0f * tr1 * d12)); // Relative angle to intersection point (radians ew)
	float d2 = atan2f((p2.y - p1.y), (p2.x - p1.x)); // Angle to next pillar to remove relativity

	float dx = p1.x + cosf(d2 + d1) * tr1;
	float dy = p1.y + sinf(d2 + d1) * tr1;

	float dx2 = p1.x + cosf(d2 - d1) * tr1;
	float dy2 = p1.y + sinf(d2 - d1) * tr1;

	

	*out1 = {dx,dy};
	*out2 = {dx2,dy2};

	if (isnan(dx) || isnan(dx2))
		return false;
	return true;
}
int main() {
	setup(2, 2, Width, Height);
	POINT mouse;
	while (1) {
		GetMousePos(&mouse);
		px = mouse.x;
		py = mouse.y;

		// Get distance to each pillar
		dist1 = dist(px, py, pillar1.x, pillar1.y);
		dist2 = dist(px, py, pillar2.x, pillar2.y);
		dist3 = dist(px, py, pillar3.x, pillar3.y);

		dist1 -= 2.0f; // Give a bit of inaccuracy to the pillars
		dist2 -= 14.0f;
		dist3 += 7.0f;

		float d12 = dist(pillar1.x, pillar1.y, pillar2.x, pillar2.y); // Distance between point 1 and 2
		float d12t = (dist1 + dist2) - d12; // Collision distance between point 1 and 2
		float rx12 = (pillar2.x - pillar1.x) / d12; // Direction
		float ry12 = (pillar2.y - pillar1.y) / d12;

		clear();
		FillB(pillar1.x - 5, pillar1.y - 5, pillar1.x + 5, pillar1.y + 5, 3);
		FillB(pillar2.x - 5, pillar2.y - 5, pillar2.x + 5, pillar2.y + 5, 4);
		FillB(pillar3.x - 5, pillar3.y - 5, pillar3.x + 5, pillar3.y + 5, 5);

		if (GetLeftMouseDown()) { // Show the distance radii
			DrawCircLP(pillar1.x, pillar1.y, dist1, 1, 3);
			DrawCircLP(pillar2.x, pillar2.y, dist2, 1, 4);
			DrawCircLP(pillar3.x, pillar3.y, dist3, 1, 5);
		}
		if (GetRightMouseDown()) { // Show the calculated position from pillar distances
			vec2 p12;
			vec2 p12x;

			vec2 p23;
			vec2 p23x;

			vec2 p31;
			vec2 p31x;

			bool check1 = intersect(pillar1, pillar2, dist1, dist2, &p12, &p12x);
			bool check2 = intersect(pillar2, pillar3, dist2, dist3, &p23, &p23x);
			bool check3 = intersect(pillar3, pillar1, dist3, dist1, &p31, &p31x);
			int temp = 0;

			// Calculated position
			vec2 localPos = { 0.0f,0.0f };

			if (check1) { // Is it a number
				// Get the closest intersection point between two pillars to the third pillars radius (closest point to the actual position)
				if (fabs(dist(p12, pillar3) - dist3) < fabs(dist(p12x, pillar3) - dist3)) {
					check1 = true;
					localPos.x += p12.x;
					localPos.y += p12.y;
				}
				else {
					localPos.x += p12x.x;
					localPos.y += p12x.y;
				}
				temp++;
			}
			if (check2) {
				if (fabs(dist(p23, pillar1) - dist1) < fabs(dist(p23x, pillar1) - dist1)) {
					check2 = true;
					localPos.x += p23.x;
					localPos.y += p23.y;
				}
				else {
					localPos.x += p23x.x;
					localPos.y += p23x.y;
				}
				temp++;
			}
			if (check3) {
				if (fabs(dist(p31, pillar2) - dist2) < fabs(dist(p31x, pillar2) - dist2)) {
					check3 = true;
					localPos.x += p31.x;
					localPos.y += p31.y;
				}
				else {
					localPos.x += p31x.x;
					localPos.y += p31x.y;
				}
				temp++;
			}
			// Average the calculated position
			localPos.x /= (float)temp;
			localPos.y /= (float)temp;
			DrawCirc(localPos.x, localPos.y, 4,6);
		}
		DrawCirc(px, py, 4, 2);
		update();
	}
	return 0;
}