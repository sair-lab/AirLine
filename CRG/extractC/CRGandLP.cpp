#include <pybind11.h>
#include <numpy.h>
#include <iostream>
#define UINT8R py::detail::unchecked_mutable_reference<unsigned char,2i64>

#define PI 3.14159265
using namespace std;
namespace py = pybind11;
static int startCmd;
static long sum;
const int threadNum = 16;
static int areaList[1000][2] = { 0 };
static int listqueue = 0;
static int listcurrent = 0;
int add(int* i, int* j) {
	*i += *j;
	return *i;
}


void f1(py::array_t<int>* x)
{
	auto r = (*x).mutable_unchecked<1>();
	//while (startCmd == 0)	this_thread::sleep_for(chrono::microseconds(1));
	//while (start == 0);
	for (int i = 0; i < 10000; i++) {
		r(0) += 1;
	}

}
void test1(py::array_t<int> x) {
	auto r = x.mutable_unchecked<1>(); // x must have ndim = 3; can be non-writeable
	py::ssize_t a = 1;
	for (int i = 0; i < 2; i++)
		r[i] += 1;
}

bool ifOutOfBound(py::array_t<bool>* in, int y, int x) {
	auto r = (*in).mutable_unchecked<3>();
	return y <= 3 || x <= 3 || y >= r.shape(1) - 4 || x >= r.shape(2) - 4;
	//cout << "out";
}


float* normalize(float y, float x) {
	float result[2];
	float l2 = sqrt(y * y + x * x);
	result[0] = y / l2;
	result[1] = x / l2;


	return result;
}
double CosineSimilarity(float* v1, float* v2) {
	float* normalized = normalize(v1[0], v1[1]);
	float* normalized2 = normalize(v2[0], v2[1]);


	return normalized[0] * normalized2[0] + normalized[1] * normalized2[1];
}

int DesGrow1(py::array_t<uint8_t> edge, py::array_t<int> usedMap, py::array_t<float> Des, py::array_t<float>Out, float similarityThreshold, int leastNumofPx, py::array_t<int>_tm, py::array_t<int>_tm2, py::array_t<float>_tm3, int NumOfDims) {
	auto e = edge.mutable_unchecked<3>();
	auto u = usedMap.mutable_unchecked<2>();
	auto outputPointL = Out.mutable_unchecked<3>();
	auto _o = _tm3.mutable_unchecked<3>();
	auto d = Des.mutable_unchecked<3>();
	auto m = _tm.mutable_unchecked<2>();
	auto m2 = _tm2.mutable_unchecked<3>();


	int minUnusedNum = 2;
	int center[2] = { 0,0 };
	int count = 0;
	//int candidate[2][300][2] = { 0 };
	int candidateNum[2] = { 0 };
	float tempDes[999] = { 0 };
	bool pingpong = 0;
	float avgDes[999] = { 0 };
	int lineNum = 0;
	int ty = 0;
	int tx = 0;
	float centerOfMass[2] = { 0 };
	for (int y = 2; y < (e.shape(0) - 3); y += 2) {//skip first line 
		for (int x = 2; x < (e.shape(1) - 3); x += 2) {

			candidateNum[pingpong] = 0;
			candidateNum[!pingpong] = 0;
			bool searchEnd = 0;

			if (u(y, x) > 0) {//should be start of a new region
				//cout << "new search at" << y << "," << x << endl;
				u(y, x) -= 1;//mark as used

				for (int c = 0; c < NumOfDims; c++) {
					avgDes[c] = d(c, y, x);
				}

				m2(pingpong, candidateNum[pingpong], 0) = y;//candidate[pingpong][candidateNum[pingpong]][0] = y;
				m2(pingpong, candidateNum[pingpong], 1) = x;//candidate[pingpong][candidateNum[pingpong]][1] = x;
				centerOfMass[0] = y;
				centerOfMass[1] = x;
				candidateNum[pingpong]++;
				count = 0;//total count of the region
				m(count, 0) = y;
				m(count, 1) = x;
				count++;
				while (searchEnd == 0) {
					//if(candidateNum[pingpong]>1)					cout << "candidatenum=" << candidateNum[pingpong] << endl;

					for (int i = 0; i < candidateNum[pingpong]; i++) {//search in the last list
						int cy = m2(pingpong, i, 0);//y of candidate
						int cx = m2(pingpong, i, 1);//x of candidate
						if (!(cy <= 1 || cy > (e.shape(0) - 2) || cx <= 1 || cx > (e.shape(1) - 2)))
							for (int dy = -1; dy <= 1; dy++) {// search around candidate and add to candidate list
								for (int dx = -1; dx <= 1; dx++) {
									if (u(cy + dy, cx + dx) > 0) {//if not used

										for (int c = 0; c < NumOfDims; c++) {//read des
											tempDes[c] = d(c, cy + dy, cx + dx);
										}

										float cosSim = 0;
										int largestDim = 0;
										int largestAvgDim = 0;
										float _temp = 0;
										float _temp2 = 0;
										int secondLarge = 0;
										for (int c = 0; c < NumOfDims; c++) {//read des
											if (_temp < tempDes[c]) {
												_temp = tempDes[c];
												largestDim = c;
											}
											if (_temp2 < avgDes[c]) {
												secondLarge = largestAvgDim;
												_temp2 = avgDes[c];
												largestAvgDim = c;
											}
											cosSim += avgDes[c] * tempDes[c];
										}
										//cout << largestAvgDim<<","<< largestDim << endl;
										int ifSimilar = 0;
										for (int i = largestAvgDim - 1; i <= largestAvgDim + 1; i++) {
											int dim = i % NumOfDims;
											ifSimilar += (abs(avgDes[dim] - tempDes[dim]) < similarityThreshold);
										}
										if (cosSim > similarityThreshold) {//cosSim > similarityThreshold//abs(avgDes[largestAvgDim]- tempDes[largestDim])< similarityThreshold//abs(largestAvgDim - largestDim) < 2
											u(cy + dy, cx + dx) = 0;//mark as used
											//&& abs(avgDes[secondLarge] - tempDes[secondLarge]) < similarityThreshold 
											//centerOfMass[0] = (centerOfMass[0] * count + cy + dy) / (count + 1);
											//centerOfMass[1] = (centerOfMass[1] * count + cx + dx) / (count + 1);


											m2(!pingpong, candidateNum[!pingpong], 0) = cy + dy;//add candidate to the other list
											m2(!pingpong, candidateNum[!pingpong], 1) = cx + dx;
											candidateNum[!pingpong]++;

											//tempGradient[0] *= ((tempGradient[0] * avgGradient[0]) < 0);//inverse
											//tempGradient[1] *= ((tempGradient[1] * avgGradient[1]) < 0);
											for (int c = 0; c < NumOfDims; c++) {//read des
												avgDes[c] = (avgDes[c] * count + tempDes[c]) / (count + 1);//update avg gradient
											}

											float l2 = 0;
											for (int c = 0; c < NumOfDims; c++) {//read des
												l2 += avgDes[c] * avgDes[c];
											}
											l2 = sqrt(l2);
											for (int c = 0; c < NumOfDims; c++) {//read des
												avgDes[c] /= l2;
											}
											//cout << "here" << endl;

											//cout << "avgg=" << avgGradient[0] << "," << avgGradient[1]<<endl;
											if (count < 5000) {
												m(count, 0) = cy + dy;
												m(count, 1) = cx + dx;
												count++;
											}
											else {
												searchEnd = 1;//invalid area to large
											}



										}
									}
								}
							}
					}

					candidateNum[pingpong] = 0;

					if (candidateNum[!pingpong] == 0) {//no more candidate
						searchEnd = 1;
						float vy = 0;
						float vx = 0;
						float _dy = 0;
						float _dx = 0;
						float nmaxLength = 0;
						float pmaxLength = 0;
						int r = rand() * 255;
						int g = rand() * 255;
						int b = rand() * 255;
						if (count > leastNumofPx && count < 5000) {
							//cout << count << endl;
							centerOfMass[0] = 0;
							centerOfMass[1] = 0;
							for (int p = 0; p < count; p++) {

								centerOfMass[0] += m(p, 0);
								centerOfMass[1] += m(p, 1);
							}
							centerOfMass[0] /= count;
							centerOfMass[1] /= count;
							for (int p = 0; p < count; p++) {
								//cout << count << endl;
								int _x = m(p, 1);
								int _y = m(p, 0);
								//u(m(p, 0), m(p, 1)) = 1;
								e(_y, _x, 0) = r;
								e(_y, _x, 1) = g;
								e(_y, _x, 2) = b;
								_dy = centerOfMass[0] - _y;
								_dx = centerOfMass[1] - _x;
								float length = sqrt(_dy * _dy + _dx * _dx);
								if ((vy * _dy + vx * _dx) < 0) {
									_dy = -_dy;
									_dx = -_dx;
									if (length > nmaxLength)nmaxLength = length;
								}
								else {
									if (length > pmaxLength)pmaxLength = length;
								}
								vy += _dy;// *length;
								vx += _dx;// *length;
							}
							float l2 = sqrt(vy * vy + vx * vx);
							vy /= l2;
							vx /= l2;
							if (vy < 0) {
								vy = -vy;
								vx = -vx;
							}
							float straightScore = 0;
							float finalDirection[2] = { vy,vx };
							float _c = 0;
							for (int p = 0; p < count; p += 3) {//measure curvature
								_c++;
								//cout << count << endl;
								int _x = m(p, 1);
								int _y = m(p, 0);
								//u(m(p, 0), m(p, 1)) = 1;
								_dy = centerOfMass[0] - _y;
								_dx = centerOfMass[1] - _x;
								float pixelDirection[2] = { _dy, _dx };
								
								float cosS = CosineSimilarity(pixelDirection, finalDirection);
								cosS = abs(cosS);
								
								straightScore += cosS;
							}
							straightScore= straightScore*100/_c;

							outputPointL(lineNum, 0, 0) = centerOfMass[0] - vy * pmaxLength;
							outputPointL(lineNum, 0, 1) = centerOfMass[1] - vx * pmaxLength;
							outputPointL(lineNum, 0, 2) = straightScore;
							outputPointL(lineNum, 1, 0) = centerOfMass[0] + vy * nmaxLength;
							outputPointL(lineNum, 1, 1) = centerOfMass[1] + vx * nmaxLength;
							outputPointL(lineNum, 1, 2) = straightScore;
							lineNum++;
							//cout << "search end with count" << count << endl;
						}
					}
					else {
						pingpong = !pingpong;//switch to another list
						//cout << "switch" << endl;
					}

				}

			}



		}
	}

	
	return lineNum;
}

PYBIND11_MODULE(CRG, m) {
	//py::register_exception<std::exception>(m, "PyExp");
	m.doc() = "pybind11 example plugin"; // optional Bmodule docstring

	m.def("add", &add, "A function that adds two numbers");
	m.def("f1", &f1, "A function that adds two numbers");
	m.def("desGrow", &DesGrow1, "extract center mass point");


}