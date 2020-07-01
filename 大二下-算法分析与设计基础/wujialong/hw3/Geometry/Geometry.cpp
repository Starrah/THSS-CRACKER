#include "Geometry.h"
using namespace std;

double dist(const Point & A, const Point & B) {
	return sqrt(sqr(A.x - B.x) + sqr(A.y - B.y));
}

pair<int, int> solve_closest(const Point * p, const int * X, const int * Y, int l, int r) {
	if (r - l + 1 <= 3) {
		int p1, p2; double mind = inf, tmp;
		for (int i = l; i <= r; i++)
			for (int j = i + 1; j <= r; j++)
				if ((tmp = dist(p[X[i]], p[X[j]])) < mind)
					mind = tmp, p1 = X[i], p2 = X[j];
		return make_pair(p1, p2);
	}
	int mid = (l + r) / 2, n = r - l + 1;
	int *Yl = new int[mid - l + 2], *Yr = new int[r - mid + 1], cnt1 = 0, cnt2 = 0;
	for (int i = 1; i <= n; i++)
		if (p[Y[i]].x < p[X[mid]].x || (p[Y[i]].x == p[X[mid]].x && Y[i] <= X[mid]))
			Yl[++cnt1] = Y[i];
		else
			Yr[++cnt2] = Y[i];

	pair<int, int> resL = solve_closest(p, X, Yl, l, mid); double dL = dist(p[resL.first], p[resL.second]);
	pair<int, int> resR = solve_closest(p, X, Yr, mid + 1, r); double dR = dist(p[resR.first], p[resR.second]);
	pair<int, int> res; double d, tmp;
	if (dL < dR) {
		res = resL; d = dL;
	}
	else {
		res = resR; d = dR;
	}

	int *Ym = new int[r - l + 2], cnt = 0;
	for (int i = 1; i <= n; i++)
		if (abs(p[Y[i]].x - p[X[mid]].x) <= d)
			Ym[++cnt] = Y[i];
	for (int i = 1; i <= cnt; i++)
		for (int j = i + 1; j <= i + 7 && j <= cnt; j++)
			if ((tmp = dist(p[Ym[i]], p[Ym[j]])) < d) {
				d = tmp; res = make_pair(Ym[i], Ym[j]);
			}
	delete[] Yl; delete[] Yr; delete[] Ym;
	return res;
}

pair<int, int> closest_divide_and_conquer(const Point * p, int n) {
	int *X = new int[n + 1], *Y = new int[n + 1];
	for (int i = 1; i <= n; i++) X[i] = Y[i] = i;
	sort(X + 1, X + n + 1, [&](int a, int b) { return p[a].x == p[b].x ? a < b : p[a].x < p[b].x; });
	sort(Y + 1, Y + n + 1, [&](int a, int b) { return p[a].y == p[b].y ? a < b : p[a].y < p[b].y; });
	pair<int,int> res = solve_closest(p, X, Y, 1, n);
	delete[] X; delete[] Y;
	return res;
}

pair<int, int> closest_brute(const Point * p, int n) {
	int p1, p2; double mind = inf, tmp;
	for (int i = 1; i <= n; i++)
		for (int j = i + 1; j <= n; j++)
			if ((tmp = dist(p[i], p[j])) < mind)
				mind = tmp, p1 = i, p2 = j;
	return make_pair(p1, p2);
}
