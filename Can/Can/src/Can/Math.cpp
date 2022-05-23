#include "canpch.h"
#include "Math.h"

#undef min
#undef max
namespace Can::Math
{
	std::vector<v3> GetCubicCurveSamples(const std::array<v3, 4>& vs, f32 preferedLength, std::vector<f32>& ts)
	{
		v3 startP = vs[0];

		while (true)
		{
			f32 l = glm::length(vs[3] - startP[0]);

			f32 prevT = ts[ts.size() - 1];
			f32 t = 1.0f;

			while (l > preferedLength * 0.8f)
			{
				t = (t + prevT) * 0.5f;
				v3 p = Math::CubicCurve<f32>(vs, t);
				l = glm::length(p - startP);
			}
			t += (t - prevT);
			while (l > preferedLength * 0.8f)
			{
				t -= (t - prevT) * 0.1f;
				v3 p = Math::CubicCurve<f32>(vs, t);
				l = glm::length(p - startP);
			}
			t += (t - prevT) * 0.1f;

			if (t > 0.99f && l < preferedLength * 0.5f)
			{
				ts.pop_back();
				ts.push_back(1.0f);
				break;
			}
			else
			{
				ts.push_back(t);
				startP = Math::CubicCurve<f32>(vs, t);
			}
		}
		u64 Size = ts.size();
		std::vector<v3> points;
		points.resize(Size);
		points[0] = vs[0];
		for (u64 i = 1; i < Size - 1; i++)
			points[i] = Math::CubicCurve<f32>(vs, ts[i]);
		points[Size - 1] = vs[3];

		if (Size < 2) return { vs[0], vs[3] };

		constexpr u64 Quality = 2;
		for (u64 i = 0; i < Quality; i++)
		{
			f32 avgLength = 0.0f;
			for (u64 i = 0; i < Size - 1; i++)
				avgLength += glm::length(points[i] - points[i + 1]);
			avgLength /= (Size - 1.0f);

			for (u64 i = 1; i < Size - 1; i++)
			{
				f32 l = glm::length(points[i] - points[i - 1]);
				f32 ratio = (l + avgLength) / (l * 2.0f);
				if (ratio <= 0.0f)
					continue;
				f32 nextT = (ts[i] - ts[i - 1]) * ratio + ts[i - 1];
				if (nextT >= ts[i + 1])
					continue;
				ts[i] = nextT;
				points[i] = CubicCurve(vs, ts[i]);
			}
			f32 l = glm::length(points[Size - 1] - points[Size - 2]);
			f32 ratio = (l + avgLength) / (l * 2.0f);
			ts[Size - 2] = 1.0f - (1.0f - ts[Size - 2]) * ratio;
			points[Size - 2] = CubicCurve(vs, ts[Size - 2]);
		}
		return points;
	}
	std::vector<f32> GetCubicCurveSampleTs(const std::array<v3, 4>& vs, f32 preferedLength)
	{
		std::vector<f32> result{ 0.0f };
		v3 startP = vs[0];

		while (true)
		{
			f32 l = glm::length(vs[3] - startP[0]);

			f32 prevT = result[result.size() - 1];
			f32 t = 1.0f;

			while (l > preferedLength * 0.8f)
			{
				t = (t + prevT) * 0.5f;
				v3 p = Math::CubicCurve<f32>(vs, t);
				l = glm::length(p - startP);
			}
			t += (t - prevT);
			while (l > preferedLength * 0.8f)
			{
				t -= (t - prevT) * 0.1f;
				v3 p = Math::CubicCurve<f32>(vs, t);
				l = glm::length(p - startP);
			}
			t += (t - prevT) * 0.1f;

			if (t > 0.99f && l < preferedLength * 0.5f)
			{
				result.pop_back();
				result.push_back(1.0f);
				break;
			}
			else
			{
				result.push_back(t);
				startP = Math::CubicCurve<f32>(vs, t);
			}
		}
		u64 Size = result.size();
		std::vector<v3> points;
		points.resize(Size);
		points[0] = vs[0];
		for (u64 i = 1; i < Size - 1; i++)
			points[i] = Math::CubicCurve<f32>(vs, result[i]);
		points[Size - 1] = vs[3];

		if (Size < 2) return result;

		constexpr u64 Quality = 2;
		for (u64 i = 0; i < Quality; i++)
		{
			f32 avgLength = 0.0f;
			for (u64 i = 0; i < Size - 1; i++)
				avgLength += glm::length(points[i] - points[i + 1]);
			avgLength /= (Size - 1.0f);

			for (u64 i = 1; i < Size - 1; i++)
			{
				f32 l = glm::length(points[i] - points[i - 1]);
				f32 ratio = (l + avgLength) / (l * 2.0f);
				if (ratio <= 0.0f)
					continue;
				f32 nextT = (result[i] - result[i - 1]) * ratio + result[i - 1];
				if (nextT >= result[i + 1])
					continue;
				result[i] = nextT;
				points[i] = CubicCurve(vs, result[i]);
			}
			f32 l = glm::length(points[Size - 1] - points[Size - 2]);
			f32 ratio = (l + avgLength) / (l * 2.0f);
			result[Size - 2] = 1.0f - (1.0f - result[Size - 2]) * ratio;
			points[Size - 2] = CubicCurve(vs, result[Size - 2]);
		}
		return result;
	}

	bool CheckPointTriangleCollision(const std::array<v2, 3>& triangleA, const v2& point)
	{
		v2 u = (triangleA[2] - triangleA[1]) - (glm::dot(triangleA[2] - triangleA[0], triangleA[2] - triangleA[1]) / glm::dot(triangleA[2] - triangleA[0], triangleA[2] - triangleA[0])) * (triangleA[2] - triangleA[0]);
		v2 v = (triangleA[1] - triangleA[0]) - (glm::dot(triangleA[1] - triangleA[2], triangleA[1] - triangleA[0]) / glm::dot(triangleA[1] - triangleA[2], triangleA[1] - triangleA[2])) * (triangleA[1] - triangleA[2]);

		f32 a = 1 - glm::dot(v, point - triangleA[0]) / glm::dot(v, triangleA[1] - triangleA[0]);
		f32 b = 1 - glm::dot(u, point - triangleA[1]) / glm::dot(u, triangleA[2] - triangleA[1]);

		return a > 0 && a < 1 && b > 0 && b < 1 && a + b < 1;
	}
	bool CheckTriangleTriangleCollision(const std::array<v2, 3>& triangleA, const std::array<v2, 3>& triangleB)
	{
		for (u64 i = 0; i < 3; i++)
			if (CheckPointTriangleCollision(triangleA, triangleB[i]))
				return true;

		for (u64 i = 0; i < 3; i++)
			if (CheckPointTriangleCollision(triangleB, triangleA[i]))
				return true;

		for (u64 i = 0; i < 3; i++)
			for (u64 j = 0; j < 3; j++)
				if (CheckLineSegmentLineSegmentCollision(
					std::array<v2, 2>{ triangleA[i], triangleA[(i + 1) % 3] },
					std::array<v2, 2>{ triangleB[j], triangleB[(j + 1) % 3] }
		))
					return true;


		return false;
	}
	bool CheckPolygonPointCollision(const std::vector<std::array<v2, 3>>& polygon, const v2& point)
	{
		u64 count = polygon.size();
		for (u64 i = 0; i < count; i++)
			if (CheckPointTriangleCollision(polygon[i], point))
				return true;
		return false;
	}
	bool CheckLineSegmentLineSegmentCollision(const std::array<v2, 2>& lineSegmentA, const std::array<v2, 2>& lineSegmentB, v2* intersection)
	{
		f32 s_numer, t_numer, denom, t;
		v2 s10 = lineSegmentA[0] - lineSegmentA[1];
		v2 s32 = lineSegmentB[0] - lineSegmentB[1];

		denom = s10.x * s32.y - s32.x * s10.y;
		if (denom == 0)
			return false; // Collinear
		bool denomPositive = denom > 0;

		v2 s02 = lineSegmentB[0] - lineSegmentA[0];

		s_numer = s10.x * s02.y - s10.y * s02.x;
		if ((s_numer < 0) == denomPositive)
			return false; // No collision

		t_numer = s32.x * s02.y - s32.y * s02.x;
		if ((t_numer < 0) == denomPositive)
			return false; // No collision

		if (((s_numer > denom) == denomPositive) || ((t_numer > denom) == denomPositive))
			return false; // No collision
		// Collision detected
		t = t_numer / denom;
		if (intersection)
			(*intersection) = lineSegmentA[0] + (t * s10);
		return true;
	}

	v2 RotatePoint(const v2& point, f32 angle)
	{
		return v2{
			glm::cos(angle) * point.x - glm::sin(angle) * point.y,
			glm::sin(angle) * point.x + glm::cos(angle) * point.y
		};
	}
	v2 RotatePointAroundPoint(const v2& p1, f32 angle, const v2& p2)
	{
		return v2{
			glm::cos(angle) * (p1.x - p2.x) - glm::sin(angle) * (p1.y - p2.y) + p2.x,
			glm::sin(angle) * (p1.x - p2.x) + glm::cos(angle) * (p1.y - p2.y) + p2.y
		};
	}

	std::vector<v3> get_samples_from_cubic_bezier_curve(const std::array<v3, 4>& control_points, f32 preferred_length)
	{
		std::vector<v3> samples{};
		std::vector<f32> ts{};
		f32 t0 = 0.0f;
		while (true)
		{
			ts.push_back(t0);
			v3 p0 = Math::CubicCurve<f32>(control_points, t0);
			samples.push_back(p0);
			f32 l = glm::length(control_points[3] - p0[0]);

			f32 t1 = 1.0f;

			while (l > preferred_length * 0.8f)
			{
				t1 = (t1 + t0) * 0.5f;
				v3 p1 = Math::CubicCurve<f32>(control_points, t1);
				l = glm::length(p1 - p0);
			}
			t1 += (t1 - t0);
			while (l > preferred_length * 0.8f)
			{
				t1 -= (t1 - t0) * 0.1f;
				v3 p1 = Math::CubicCurve<f32>(control_points, t1);
				l = glm::length(p1 - p0);
			}
			t1 += (t1 - t0) * 0.1f;

			t0 = t1;
			if (t1 > 0.99f && l < preferred_length * 0.5f)
				break;
		}
		ts.push_back(1.0f);
		samples.push_back(control_points[3]);
		u64 sample_count = samples.size();
		if (sample_count <= 2)
			return samples;

		constexpr u64 Quality = 2;
		for (u64 k = 0; k < Quality; k++)
		{
			f32 average_length = 0.0f;
			for (u64 i = 0; i < sample_count - 1; i++)
				average_length += glm::length(samples[i] - samples[i + 1]);
			average_length /= (sample_count - 1.0f);

			for (u64 i = 0; i < sample_count - 2; i++)
			{
				f32 l = glm::length(samples[i + 1] - samples[i]);
				if (l > 2.2f * average_length)
				{
					f32 inserted_t = 0.5f * (ts[i] + ts[i + 1]);

					auto samples_iterator = samples.begin();
					std::advance(samples_iterator, i + 1);
					samples.insert(samples_iterator, Math::CubicCurve<f32>(control_points, inserted_t));

					auto ts_iterator = ts.begin();
					std::advance(ts_iterator, i + 1);
					ts.insert(ts_iterator, inserted_t);

					sample_count++;
					i--;
					continue;
				}
				f32 ratio = (2.0f * l + average_length) / (l * 3.0f);

				f32 nextT = lerp(ts[i], ts[i + 1], ratio);
				if (nextT >= ts[i + 2])
				{
					auto samples_iterator = samples.begin();
					std::advance(samples_iterator, i + 1);
					samples.erase(samples_iterator);

					auto ts_iterator = ts.begin();
					std::advance(ts_iterator, i + 1);
					ts.erase(ts_iterator);

					sample_count--;
					i--;
					continue;
				}
				ts[i + 1] = nextT;
				samples[i + 1] = CubicCurve(control_points, ts[i + 1]);
			}
		}
		return samples;
	}

	std::array<v3, 2> get_bounding_box_from_straight_bezier_curve(const std::array<v3, 4>& control_points, f32 half_width, f32 height)
	{
		v3 AB = control_points[1] - control_points[0];
		v3 DC = control_points[2] - control_points[3];
		AB = half_width * glm::normalize(v3{ -AB.y , AB.x, 0.0f });
		DC = half_width * glm::normalize(v3{ -DC.y , DC.x, 0.0f });

		auto mins_and_maxs = Math::GetMinsAndMaxs(std::array<v3, 8>{
			control_points[0] + AB,
				control_points[0] - AB,
				control_points[1] + AB,
				control_points[1] - AB,
				control_points[2] + DC,
				control_points[2] - DC,
				control_points[3] + DC,
				control_points[3] - DC,
		});

		mins_and_maxs[1].z += height;
		return mins_and_maxs;
	}

	std::array<v3, 2> get_bounding_box_from_cubic_bezier_curve(const std::array<v3, 4>& control_points, f32 half_width, f32 height)
	{
		v3 mins = {
			std::min({ control_points[0].x, control_points[1].x, control_points[2].x, control_points[3].x}) - half_width,
			std::min({ control_points[0].y, control_points[1].y, control_points[2].y, control_points[3].y}) - half_width,
			std::min({ control_points[0].z, control_points[1].z, control_points[2].z, control_points[3].z})
		};
		v3 maxs = {
			std::max({ control_points[0].x, control_points[1].x, control_points[2].x, control_points[3].x}) + half_width,
			std::max({ control_points[0].y, control_points[1].y, control_points[2].y, control_points[3].y}) + half_width,
			std::max({ control_points[0].z, control_points[1].z, control_points[2].z, control_points[3].z}) + height
		};
		return { mins, maxs };
	}

	std::vector<std::array<v3, 3>> get_bounding_polygon_from_bezier_curve(const std::array<v3, 4>& control_points, f32 half_width, f32 preferred_length)
	{
		std::vector<v3> samples = get_samples_from_cubic_bezier_curve(control_points, preferred_length);
		return get_bounding_polygon_from_bezier_curve(samples, half_width);
	}

	std::vector<std::array<v3, 3>> get_bounding_polygon_from_bezier_curve(const std::vector<v3>& samples, f32 half_width)
	{
		std::vector<std::array<v3, 3>> result{};
		u64 sample_count = samples.size();
		result.reserve((sample_count - 1) * 2);

		v3 p0 = samples[0];
		for (u64 i = 1; i < sample_count; i++)
		{
			v3 p1 = samples[i];
			v3 direction = p1 - p0;
			direction = v3{ -direction.y, direction.x, 0.0f };
			direction = half_width * glm::normalize(direction);

			result.push_back({
					p0 + direction,
					p0 - direction,
					p1 + direction
				});
			result.push_back({
					p0 - direction,
					p1 - direction,
					p1 + direction
				});
		}
		return result;
	}

	bool check_bounding_box_bounding_box_collision(std::array<v3, 2> bounding_box_1, std::array<v3, 2> bounding_box_2)
	{
		bool is_x_axis_seperated = bounding_box_1[0].x > bounding_box_2[1].x || bounding_box_2[0].x > bounding_box_1[1].x;
		bool is_y_axis_seperated = bounding_box_1[0].y > bounding_box_2[1].y || bounding_box_2[0].y > bounding_box_1[1].y;
		bool is_z_axis_seperated = bounding_box_1[0].z > bounding_box_2[1].z || bounding_box_2[0].z > bounding_box_1[1].z;
		return !(is_x_axis_seperated || is_y_axis_seperated || is_z_axis_seperated);
	}

	bool check_bounding_polygon_bounding_polygon_collision_with_z(std::vector<std::array<v3, 3>> polygon_1, f32 polygon_1_height, std::vector<std::array<v3, 3>> polygon_2, f32 polygon_2_height)
	{
		u64 triangle_count_of_polygon_1 = polygon_1.size();
		u64 triangle_count_of_polygon_2 = polygon_2.size();

		for (u64 i = 0; i < triangle_count_of_polygon_1; i++)
			for (u64 j = 0; j < triangle_count_of_polygon_2; j++)
				//if (check_triangle_triangle_collision_with_z(polygon_1[i], polygon_1_height, polygon_2[j], polygon_2_height)) return true;
				if (check_triangle_triangle_collision_with_SAT(polygon_1[i], polygon_2[j]))
					if (!((polygon_1[i][0].z > polygon_2[j][0].z + polygon_2_height) || (polygon_2[j][0].z > polygon_1[i][0].z + polygon_1_height)))
						return true;

		return false;
	}

	bool check_triangle_triangle_collision_with_z(const std::array<v3, 3>& triangle_1, f32 triangle_1_height, const std::array<v3, 3>& triangle_2, f32 triangle_2_height)
	{
		bool has_a_collision_in_xy_plane = false;
		for (u64 i = 0; i < 3; i++)
		{
			if (check_triangle_point_collision(triangle_1, triangle_2[i]))
			{
				has_a_collision_in_xy_plane = true;
				break;
			}
		}
		if (!has_a_collision_in_xy_plane)
		{
			for (u64 i = 0; i < 3; i++)
			{
				if (check_triangle_point_collision(triangle_2, triangle_1[i]))
				{
					has_a_collision_in_xy_plane = true;
					break;
				}
			}
		}

		if (!has_a_collision_in_xy_plane)
		{
			for (u64 i = 0; i < 3; i++)
			{
				for (u64 j = 0; j < 3; j++)
				{
					if (check_line_segment_line_segment_collision(
						std::array<v3, 2>{ triangle_1[i], triangle_1[(i + 1) % 3] },
						std::array<v3, 2>{ triangle_2[j], triangle_2[(j + 1) % 3] }))
					{
						has_a_collision_in_xy_plane = true;
						break;
					}
				}
			}
		}
		bool has_a_collision_in_z_axis = !((triangle_1[0].z > triangle_2[0].z + triangle_2_height) || (triangle_2[0].z > triangle_1[0].z + triangle_1_height));
		return has_a_collision_in_z_axis && has_a_collision_in_xy_plane;
	}

	bool check_triangle_point_collision(const std::array<v3, 3>& triangle, v3 point)
	{
		v3 AB = triangle[1] - triangle[0];
		v3 AC = triangle[2] - triangle[0];
		v3 BC = triangle[2] - triangle[1];
		AB.z = 0.0f;
		AC.z = 0.0f;
		BC.z = 0.0f;

		v3 u = BC - (glm::dot(+AC, +BC) / glm::dot(+AC, +AC)) * +AC;
		v3 v = AB - (glm::dot(-BC, +AB) / glm::dot(-BC, -BC)) * -BC;

		f32 a = glm::dot(v, point - triangle[0]) / glm::dot(v, AB);
		f32 b = glm::dot(u, point - triangle[1]) / glm::dot(u, BC);

		bool has_collision_in_xy_plane = a > 0 && a < 1 && b > 0 && b < 1 && a + b < 1;
		return has_collision_in_xy_plane;
	}

	bool check_line_segment_line_segment_collision(const std::array<v3, 2>& line_segment_1, const std::array<v3, 2>& line_segment_2)
	{

		f32 s_numer, t_numer, denom, t;
		v3 s10 = line_segment_1[0] - line_segment_1[1];
		v3 s32 = line_segment_2[0] - line_segment_2[1];

		denom = s10.x * s32.y - s32.x * s10.y;
		if (denom == 0)
			return false; // Collinear
		bool denomPositive = denom > 0;

		v3 s02 = line_segment_2[0] - line_segment_1[0];

		s_numer = s10.x * s02.y - s10.y * s02.x;
		if ((s_numer < 0) == denomPositive)
			return false; // No collision

		t_numer = s32.x * s02.y - s32.y * s02.x;
		if ((t_numer < 0) == denomPositive)
			return false; // No collision

		if (((s_numer > denom) == denomPositive) || ((t_numer > denom) == denomPositive))
			return false; // No collision
		// Collision detected
		t = t_numer / denom;
		return true;
		/*	v3 AB = line_segment_1[1] - line_segment_1[0];
			v3 CD = line_segment_2[1] - line_segment_2[0];

			if (AB.x == 0.0f && CD.x == 0.0f)
				return false;
			if (AB.x == 0.0f)
				AB.x = 0.0001f;
			if (CD.x == 0.0f)
				CD.x = 0.0001f;

			if (AB.y / AB.x == CD.y / CD.x)
				return false;
			f32 AB_length = glm::length((v2)AB);
			f32 CD_length = glm::length((v2)CD);

			v3 v = AB;
			v.z = 0.0f;
			v = glm::normalize(v);
			v3 n = CD;
			n = v3{ -n.y, n.x, 0.0f };

			n = glm::normalize(n);
			v3 intersection_point = ray_plane_intersection(line_segment_1[0], v, line_segment_2[0], n);

			f32 AI_length = glm::length((v2)(intersection_point - line_segment_1[0]));
			f32 BI_length = glm::length((v2)(intersection_point - line_segment_1[1]));
			f32 CI_length = glm::length((v2)(intersection_point - line_segment_2[0]));
			f32 DI_length = glm::length((v2)(intersection_point - line_segment_2[1]));

			if (AB_length < AI_length)
				return false;
			if (AB_length < BI_length)
				return false;
			if (CD_length < CI_length)
				return false;
			if (CD_length < DI_length)
				return false;
			f32 ratio = AI_length / AB_length;

			f32 z1 = line_segment_1[0].z + ratio * AB.z;
			f32 z2 = line_segment_2[0].z + ratio * CD.z;

			return !(z1 + line_segment_1_height < z2 || z1 > z2 + line_segment_2_height);
		*/
	}

	v3 lerp(const v3& a, const v3& b, f32 t)
	{
		return a + (b - a) * t;
	}

	f32 lerp(f32 a, f32 b, f32 t)
	{
		return a + (b - a) * t;
	}

	v3 safe_ray_plane_intersection(v3 X, v3 v, v3 C, v3 n)
	{
		f32 dot_product = glm::dot(v, n);
		f32 len_product = glm::length(v) * glm::length(n);
		f32 cos_value = dot_product / len_product;
		f32 minned_maxxed = std::max(-1.0f, std::min(cos_value, 1.0f));
		f32 angleDiff = glm::acos(minned_maxxed);
		if (angleDiff < glm::radians(2.5f) || angleDiff > glm::radians(177.5f))
			v = glm::rotateZ(v, glm::radians(2.5f));

		return RayPlaneIntersection(X, v, C, n);
	}

	v3 ray_plane_intersection(const v3& X, const v3& v, const v3& C, const v3& n)
	{
		return RayPlaneIntersection(X, v, C, n);
	}

	std::array<v3, 3> get_axis_of_a_triangle(const std::array<v3, 3>& triangle)
	{
		v3 AB = triangle[1] - triangle[0];
		v3 BC = triangle[2] - triangle[1];
		v3 CA = triangle[0] - triangle[2];

		return std::array<v3, 3>{
			glm::normalize(v3{ -AB.y, AB.x, 0.0f }),
				glm::normalize(v3{ -BC.y, BC.x, 0.0f }),
				glm::normalize(v3{ -CA.y, CA.x, 0.0f })
		};
	}

	bool check_triangle_triangle_collision_with_SAT(const std::array<v3, 3>& triangle_1, const std::array<v3, 3>& triangle_2)
	{
		std::array<v3, 3> axis_of_triangle_1 = get_axis_of_a_triangle(triangle_1); // Cache these in polygon func
		std::array<v3, 3> axis_of_triangle_2 = get_axis_of_a_triangle(triangle_2); // Cache these in polygon func

		for (u64 i = 0; i < 3; i++)
		{
			f32 scalers1[] = {
				glm::dot(axis_of_triangle_1[i], triangle_1[0]),
				glm::dot(axis_of_triangle_1[i], triangle_1[1]),
				glm::dot(axis_of_triangle_1[i], triangle_1[2])
			};
			f32 scalers2[] = {
				glm::dot(axis_of_triangle_1[i], triangle_2[0]),
				glm::dot(axis_of_triangle_1[i], triangle_2[1]),
				glm::dot(axis_of_triangle_1[i], triangle_2[2]),
			};


			f32 s1max = *(std::max_element(scalers1, scalers1 + 3));
			f32 s1min = *(std::min_element(scalers1, scalers1 + 3));

			f32 s2max = *(std::max_element(scalers2, scalers2 + 3));
			f32 s2min = *(std::min_element(scalers2, scalers2 + 3));

			if (s1min >= s2max || s2min >= s1max)
				return false;
		}
		for (u64 i = 0; i < 3; i++)
		{
			f32 scalers1[] = {
				glm::dot(axis_of_triangle_2[i], triangle_1[0]),
				glm::dot(axis_of_triangle_2[i], triangle_1[1]),
				glm::dot(axis_of_triangle_2[i], triangle_1[2])
			};
			f32 scalers2[] = {
				glm::dot(axis_of_triangle_2[i], triangle_2[0]),
				glm::dot(axis_of_triangle_2[i], triangle_2[1]),
				glm::dot(axis_of_triangle_2[i], triangle_2[2]),
			};


			f32 s1max = *(std::max_element(scalers1, scalers1 + 3));
			f32 s1min = *(std::min_element(scalers1, scalers1 + 3));

			f32 s2max = *(std::max_element(scalers2, scalers2 + 3));
			f32 s2min = *(std::min_element(scalers2, scalers2 + 3));

			if (s1min >= s2max || s2min >= s1max)
				return false;
		}
		return true;
	}

	std::array<std::array<v2, 3>, 2> GetBoundingBoxOfBezierCurve(const std::array<v3, 4>& Points, f32 halfRoadWidth)
	{
		v2 A1{ Points[0].x, Points[0].y };
		v2 B1{ Points[1].x, Points[1].y };
		v2 C1{ Points[2].x, Points[2].y };
		v2 D1{ Points[3].x, Points[3].y };

		v2 AB1 = halfRoadWidth * glm::normalize(B1 - A1);
		v2 DC1 = halfRoadWidth * glm::normalize(C1 - D1);

		AB1 = v2{ -AB1.y , AB1.x };
		DC1 = v2{ -DC1.y , DC1.x };

		v2 A11 = A1 + AB1;
		v2 A12 = A1 - AB1;
		v2 B11 = B1 + AB1;
		v2 B12 = B1 - AB1;
		v2 C11 = C1 + DC1;
		v2 C12 = C1 - DC1;
		v2 D11 = D1 + DC1;
		v2 D12 = D1 - DC1;

		auto [mins, maxs] = Math::GetMinsAndMaxs(std::array<v2, 8>{A11, A12, B11, B12, C11, C12, D11, D12});

		v2 P1{ mins.x, mins.y };
		v2 P2{ mins.x, maxs.y };
		v2 P3{ maxs.x, maxs.y };
		v2 P4{ maxs.x, mins.y };

		return std::array<std::array<v2, 3>, 2>{
			std::array<v2, 3>{P1, P2, P3},
				std::array<v2, 3>{P1, P3, P4}
		};
	}

	v3 RayPlaneIntersection(const v3& X, const v3& v, const v3& C, const v3& n)
	{
		v3 w = C - X;
		f32 k = glm::dot(w, n) / glm::dot(v, n);
		v3 result = X + k * v;
		return result;
	}
}
