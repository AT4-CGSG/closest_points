/* FILE NAME   : closest_points.cpp
 * PURPOSE     : Solver class for
 *               the closest pair of points problem.
 * PROGRAMMER  : Andrey Tsutsiev.
 * LAST UPDATE : 05.04.2020.
 */

#include <algorithm>
#include <iostream>
#include <string>
#include <cmath>
#include <vector>

namespace solution
{
  /* Infinity constant */
  constexpr double INF = 1e20;

  /* closest pair of points solver class */
  class nearest_points
  {
  private:
    struct point
    {
      int
        x = 0,
        y = 0;

      point() = default;
      point(const int x, const int y) : x(x), y(y) {}

      friend bool operator<(const point & a, const point & b) { return a.y < b.y; }

      friend double dist(const point & a, const point & b) { return (a.x - b.x) * (a.x - b.x) + (a.y - b.y) * (a.y - b.y); }
    };

    std::vector<point> points;
    double min_dist = INF;

    /* Recursive algortihm's part.
     * ARGUMENTS: None.
     * RETURNS: None.
     */
    auto solve(const int left, const int right) -> void
    {
      static const auto update = [&](const point & a, const point & b) -> void
      {
        const double d = dist(a, b);
        if (d < min_dist) min_dist = d;
      };

      if (right - left <= 3)
      {
        for (int i = left; i <= right; ++i)
          for (int j = i + 1; j <= right; ++j)
            update(points[i], points[j]);
        std::sort(points.begin() + left, points.begin() + right + 1);
      }
      else
      {
        const int
          mid = (left + right) >> 1,
          x_mid = points[mid].x;
        static std::vector<point> buff(points.size());

        solve(left, mid), solve(mid + 1, right);
        std::merge(points.begin() + left, points.begin() + mid + 1, points.begin() + mid + 1,
          points.begin() + right + 1, buff.begin());
        std::copy(buff.begin(), buff.begin() + right - left + 1, points.begin() + left);

        for (int i = left, cur_buff = 0; i <= right; ++i)
          if (abs(points[i].x - x_mid) < min_dist)
          {
            for (int j = cur_buff - 1; j >= 0 && points[i].y - buff[j].y < min_dist; --j)
              update(points[i], buff[j]);
            buff[cur_buff++] = points[i];
          }
      }
    } // End of 'rec' function

  public:
    /* Default class constructor */
    nearest_points() = default;

    /* Class destructor */
    ~nearest_points() { points.clear(); }

    /* Add new point function.
     * ARGUMENTS:
     *   - point:
     *       const point & p;
     * RETURNS: None.
     */
    nearest_points & operator<<( const point & p )
    {
      points.emplace_back(p);
      return *this;
    } // End of 'operator<<' function

    /* Add new point function.
     * ARGUMENTS:
     *   - point:
     *       point && p;
     * RETURNS: None.
     */
    nearest_points & operator<<( point && p )
    {
      points.emplace_back(p);
      return *this;
    } // End of 'operator<<' function

    /* Read a block of points from std::istream function.
     * ARGUMENTS:
     *   - input stream:
     *       std::istream & in;
     *   - receiver:
     *       nearest_points & solver;
     * RETURNS: None.
     */
    friend std::istream & operator>>(std::istream & in, nearest_points & solver)
    {
      int n, x, y;

      std::cin >> n;
      while (n--)
        in >> x >> y, solver << point(x, y);

      return in;
    } // End of 'operator>>' function

    /* '"Solve!" button'.
     * ARGUMENTS: None.
     * RETURNS: None.
     */
    double calculate()
    {
      std::sort(points.begin(), points.end(), [](const point & a, const point & b) { return (a.x < b.x) || (a.x == b.x && a.y < b.y); });
      solve(0, points.size() - 1);
      return min_dist == INF ? -1 : sqrt(min_dist);
    } // End of 'calculate' function
  }; // End of 'nearest_points' class
} // End of 'solution' namespace

int main()
{
  solution::nearest_points solver;

  std::cin >> solver;
  std::cout << solver.calculate();

  return 0;
}

/* END OF 'closest_points.h' FILE */
