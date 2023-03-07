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

bool ifOutOfBound(py::array_t<bool>* in,int y,int x) {
	auto r = (*in).mutable_unchecked<3>();
	return y <= 3 || x <= 3 || y >= r.shape(1) - 4 || x >= r.shape(2) - 4;
	//cout << "out";
}


int extract(py::array_t<bool> in ,py::array_t<float> out) {
	auto r = in.mutable_unchecked<3>();
	auto outarr = out.mutable_unchecked<3>();
	r(1, 1, 1) = 255;
	int listNUm = 0;
	int enterPos[2] = { 0,0 };
	int startx = 0;
	int starty = 0;
	int center[2] = { 0,0 };
	int count=0;
	int regionCount = 0;
	int ty=0;
	int tx=0;
	int regionCout = 0;
	bool firstpixel = true;
	for (int batch = 0; batch < r.shape(0); batch++) {
		for (int y = 0; y < r.shape(1); y++) {
			for (int x = 0; x < r.shape(2); x++) {
				if (r(batch, y, x) ==1 && y!=0 && x!=0) {//pixel not used
					//std::cout << "count" << count << "y" << y << "x" << x << "value" << int(r(batch, y, x)) << std::endl;
					//std::cout << "value" << int(r(batch, y, x)) << std::endl;
					r(batch, y - 2, x - 2) = 0;// prevent from returning
					r(batch, y - 2, x - 0) = 0;// prevent from returning
					r(batch, y - 2, x + 2) = 0;// prevent from returning
					r(batch, y - 0, x - 2) = 0;// prevent from returning
					r(batch, y , x ) = 0;// prevent from returning
					r(batch, y - 0, x + 2) = 0;// prevent from returning
					r(batch, y + 2, x - 2) = 0;// prevent from returning
					r(batch, y + 2, x - 0) = 0;// prevent from returning
					r(batch, y + 2, x + 2) = 0;// prevent from returning
					startx = x; starty = y;
					count = 0;
					center[0] = 0;
					center[1] = 0;
					areaList[listqueue][0] = y; areaList[listqueue][1] = x;
					if (listqueue == 999) {//find one more task
						listqueue = 0;
					}
					else {
						listqueue++;
					}
					while (listcurrent!= listqueue) {
						//cout << "current" << listcurrent << " queue:" << listqueue<<"center="<< areaList[listqueue][0];
						//searchForNeighbour(&in, center, &count, areaList[listqueue][0], areaList[listqueue][1], &listcurrent);
						//cout << "outof";

						//cout << "check1";
						for (int i = -1; i < 2; i++) {
							for (int o = -1; o < 2; o++) {
								//cout << "v" << int(r(0, y + i, x + o)) << " " << (y + i) << " " << (x + o) << "\n";
								ty = areaList[listcurrent][0] + i;
								tx = areaList[listcurrent][1] + o;
								if (r(batch, ty, tx) ==1) {
									regionCout++;
									//cout << "doing at "<< ty <<","<< tx<<"\n";
									r(batch, ty, tx) = 0;
									center[0] += ty;
									center[1] += tx;
									count++;
									if (! ifOutOfBound(&in, ty + i * 2, tx + o * 2)){
										areaList[listqueue][0] = ty + i * 2;
										areaList[listqueue][1] = tx + o * 2;
										r(batch, areaList[listqueue][0] - 2, areaList[listqueue][1] - 2) = 0;// prevent from returning
										r(batch, areaList[listqueue][0] - 2, areaList[listqueue][1] - 0) = 0;// prevent from returning
										r(batch, areaList[listqueue][0] - 2, areaList[listqueue][1] + 2) = 0;// prevent from returning
										r(batch, areaList[listqueue][0] - 0, areaList[listqueue][1] - 2) = 0;// prevent from returning
										r(batch, areaList[listqueue][0] - 0, areaList[listqueue][1] - 0) = 0;// prevent from returning
										r(batch, areaList[listqueue][0] - 0, areaList[listqueue][1] + 2) = 0;// prevent from returning
										r(batch, areaList[listqueue][0] + 2, areaList[listqueue][1] - 2) = 0;// prevent from returning
										r(batch, areaList[listqueue][0] + 2, areaList[listqueue][1] - 0) = 0;// prevent from returning
										r(batch, areaList[listqueue][0] + 2, areaList[listqueue][1] + 2) = 0;// prevent from returning
										//cout << "added " << ty + i * 2 << "," << tx + o * 2;
										if (listqueue == 999) {//find one more task
											listqueue = 0;
										}
										else {
											listqueue++;
										}
									}
									else {
										cout << ty + i * 2 << "," << tx + o * 2 << "out of bound\n";
									}
									//std::cout << " addone" << std::endl;
									//std::cout << " count" << *count << "y" << y + i << "x" << x + o << std::endl;
								}
							}
						}
						//cout << regionCout << " in this region\n";
						regionCout = 0;
						areaList[listcurrent][0] = 0;
						//cout << "check";
						if (listcurrent == 999) {//finish one task
							listcurrent = 0;
						}
						else {
							listcurrent++;
						}


						
					}
					outarr(batch, regionCount, 0) = float(center[0]) / count;
					outarr(batch, regionCount, 1) = float(center[1]) / count;
					//std::cout << "getone count:" << regionCount<<","<<count << std::endl;
					regionCount++;
					x = startx; y = starty;
					//goto endloop;
					/*
					if (firstpixel == false) {//reach first pixel in the region
						firstpixel = true;
						enterPos[0] = y;
						enterPos[1] = x;
					}
					center[0] += y;
					center[1] += x;
					count++;
					r(batch, y, x) = false;*/
				}
			}
		}
	}
	return regionCount;
}
static bool start = 0;


int segmentation(py::array_t<bool> lineMask, py::array_t<int32_t> startEnd,int lineNeedsToBeLongerThan,int maxBreak1,float density,int maxBreakTime) {
	int length = 600;
	int leastLength = lineNeedsToBeLongerThan;
	int maxBreak = maxBreak1;
	int lineNum=0;
	int currentLineLength = 0;
	int subLineLength = 0;
	float allzeros = 0;
	int zeros = 0;
	int start = 0;
	int end = 0;
	int subSeg = 0;
	bool currentOnALine = 0;
	int breakTime = 0;
	pybind11::detail::unchecked_mutable_reference<bool, 1i64> _lineMask = lineMask.mutable_unchecked<1>();
	pybind11::detail::unchecked_mutable_reference<int32_t, 2i64> _startEnd = startEnd.mutable_unchecked<2>();
	for (int i = 1; i < length; i++) {
		if (_lineMask[i] == 1 && currentOnALine==0) {//start on a line,initiate
			//cout << "newline at "<< i <<endl;
			start = i;
			subSeg = 1;
			breakTime = 0;
			currentOnALine = 1;
			currentLineLength=0;
		}
		else if (_lineMask[i] == 1 && currentOnALine == 1) {//continue on a line
			subSeg++;
			currentLineLength++;
		}
		else if (_lineMask[i] == 0 && currentOnALine == 1) {//break on a line
			allzeros++;
		}
		if (_lineMask[i] == 0 && _lineMask[i-1] ==0 && currentOnALine == 1) {//continuously break on a line
			zeros++;
		}
		else if (_lineMask[i] == 1 && _lineMask[i - 1] == 0 && currentOnALine == 1) {//break end
			zeros = 0;
			subLineLength = 0;
		}
		else if (_lineMask[i] == 0 && _lineMask[i - 1] == 1 && currentOnALine == 1) {//break start
			breakTime++;
		}
		else if (_lineMask[i] == 1 && _lineMask[i - 1] == 1 && currentOnALine == 1) {//continue line
			subLineLength++;
		}

		if (zeros == maxBreak && currentOnALine == 1) {//break large enough
			//cout << "end at " << i << endl;
			if (currentLineLength >= leastLength && allzeros/currentLineLength< density && breakTime<=maxBreakTime) {
				end = i- maxBreak-2;
				_startEnd(lineNum, 0) = start;
				_startEnd(lineNum, 1) = end;
				lineNum++;
			}
			zeros = 0;
			currentLineLength = 0;
			currentOnALine = 0;
		}

	}
	return lineNum;
}

int scan(py::array_t<uint8_t> in) {
	auto r = in.mutable_unchecked<3>();
	for (int batch = 0; batch < r.shape(0); batch++) {
		int minUnusedNum = 2;
		int center[2] = { 0,0 };
		int count[256] = { 0 };
		int startLine[256] = { 0 };
		int regionCount = 0;
		int ty = 0;
		int tx = 0;
		int regionCout = 0;
		int tobeChanged = 0;
		int newNum = 0;
		for (int i = 0; i < 256; i++) {
			startLine[i] = 99999;
		}
		for (int y = 1; y < r.shape(1); y++) {//skip first line 
			for (int x = 1; x < r.shape(2); x++) {
				if (r(batch, y, x) == 1 && r(batch, y-1, x) != 0 && r(batch, y, x-1) != 0) {
					//cout << "normal case\n";
					if (r(batch, y - 1, x) != r(batch, y, x - 1)) {//different num meet
						if (r(batch, y - 1, x) > r(batch, y, x - 1)) {
							tobeChanged = r(batch, y - 1, x);
							newNum = r(batch, y, x - 1);
						}
						else {
							newNum = r(batch, y - 1, x);
							tobeChanged = r(batch, y, x - 1);
						}
							//rewrite smaller num
						//cout << "merge case\n";
						//rewrite new number
						for (int _y = startLine[r(batch, y, x - 1)]; _y <= y; _y++) {
							for (int _x = 0; _x <= r.shape(2); _x++) {
								if (r(batch, _y, _x) == tobeChanged) {
									r(batch, _y, _x) = newNum;//r(batch, _y, _x) should be = minUnusedNum-1
								}
							}
						}
					}
					r(batch, y, x) = r(batch, y - 1, x);// inherit from top pixel

				}//found a unused pixel
				
				else if (r(batch, y, x) == 1 && r(batch, y - 1, x) != 0 && r(batch, y, x - 1) == 0) {//new start but top pixel exist
					//cout << "top case\n";
					r(batch, y, x) = r(batch, y - 1, x);
				}
				else if (r(batch, y, x) == 1 && r(batch, y - 1, x) == 0 && r(batch, y, x - 1) != 0) {//new start but top pixel exist
				   //cout << "left case\n";
					r(batch, y, x) = r(batch, y, x-1);
				}
				else if (r(batch, y, x) == 1 && r(batch, y - 1, x) == 0 && r(batch, y, x - 1) == 0) {//new start at corner
					//cout << "corner case "<< minUnusedNum<<"\n";
					r(batch, y, x) = minUnusedNum;
					if (startLine[r(batch, y, x)] > y) {
						//cout << int(r(batch, y, x )) << " starts at" << y << "\n";
						startLine[r(batch, y, x)] = y;
					}

					
				}

				else if (r(batch, y, x) == 0 && r(batch, y, x - 1) != 0) {//end of segment
					//cout << "end case\n";
					if (r(batch, y, x-1) == minUnusedNum) {
						minUnusedNum++;
						//cout << "min++ at " << y << "\n";
					}
				};//f

				if (minUnusedNum == 255) {
					//cout << "out!\n";
					goto quitthis;
				}
			}
		}
	}
	quitthis:
	return 1;
}

int gradientGrow(py::array_t<float> in, py::array_t<bool> usedMap, py::array_t<int> out,float similarityThreshold,int leastNumofPx) {
	auto g = in.mutable_unchecked<3>();
	auto u = usedMap.mutable_unchecked<2>();
	auto o = out.mutable_unchecked<2>();

	int minUnusedNum = 2;
	int center[2] = { 0,0 };
	int count=0;
	int candidate[2][2000][2] = { 0 };
	int all[256][2] = { 0 };
	int candidateNum[2] = {0};
	bool pingpong = 0;
	float avgGradient[2] = { 0 };
	int regionCount = 0;
	int ty = 0;
	int tx = 0;
	int regionCout = 0;
	for (int y = 2; y < (g.shape(1)-2); y++) {//skip first line 
		for (int x = 2; x < (g.shape(2)-2); x++) {

			candidateNum[pingpong] = 0;
			candidateNum[!pingpong] = 0;
			bool searchEnd = 0;

			if (u(y, x) == 1) {//should be start of a new region
				//cout << "new search at" << y << "," << x << endl;
				u(y, x) = 0;//mark as used
				avgGradient[0] = g(0, y, x);
				avgGradient[1] = g(1, y, x);

				candidate[pingpong][candidateNum[pingpong]][0] = y;
				candidate[pingpong][candidateNum[pingpong]][1] = x;
				candidateNum[pingpong]++;
				count=1;//total count of the region
				while (searchEnd == 0) {
					//if(candidateNum[pingpong]>1)					cout << "candidatenum=" << candidateNum[pingpong] << endl;


					for (int i = 0; i < candidateNum[pingpong]; i++) {//search in the last list
						int cy = candidate[pingpong][i][0];//y of candidate
						int cx = candidate[pingpong][i][1];//x of candidate
						if(!(cy<=1 || cy> (g.shape(1) - 2)|| cx <= 1 || cx > (g.shape(2) - 2)))
						for (int dy = -1; dy <= 1; dy++) {// search around candidate and add to candidate list
							for (int dx = -1; dx <= 1; dx++) {
								if (u(cy + dy, cx + dx) == 1) {//if not used
									u(cy + dy, cx + dx) = 0;//mark as used

									float tempGradient[2];
									tempGradient[0] = g(0, cy + dy, cx + dx);
									tempGradient[1] = g(1, cy + dy, cx + dx);



									float cosSim = (tempGradient[0] * avgGradient[0] + tempGradient[1] * avgGradient[1]);
									if ((cosSim > similarityThreshold) ) {//cosSim > similarityThreshold
										//o(cy + dy, cx + dx) = 1;
										if (count < 255) {
											all[count][0] = cy + dy;
											all[count][1] = cx + dx;
										}
										else {
											o(cy + dy, cx + dx) = 255;
										}

										candidate[!pingpong][candidateNum[!pingpong]][0] = cy + dy;//add candidate to the other list
										candidate[!pingpong][candidateNum[!pingpong]][1] = cx + dx;
										candidateNum[!pingpong]++;

										//tempGradient[0] *= ((tempGradient[0] * avgGradient[0]) < 0);//inverse
										//tempGradient[1] *= ((tempGradient[1] * avgGradient[1]) < 0);

										avgGradient[0] = (avgGradient[0] * count + tempGradient[0]) / (count + 1);//update avg gradient
										avgGradient[1] = (avgGradient[1] * count + tempGradient[1]) / (count + 1);

										float l2 = sqrt(avgGradient[0] * avgGradient[0] + avgGradient[1] * avgGradient[1]);
										avgGradient[0] /= l2;
										avgGradient[1] /= l2;

										//cout << "avgg=" << avgGradient[0] << "," << avgGradient[1]<<endl;

										count++;


									}
								}
							}
						}
					}

					candidateNum[pingpong] = 0;

					if (candidateNum[!pingpong] == 0) {
						searchEnd = 1;//no more candidate
						if (count > leastNumofPx) {
							count = 255;
							for (int i = 0; i < count; i++) {
								o(all[i][0], all[i][1]) = 255;
							}
							regionCount++;
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
quitthis:
	return regionCount;
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
int straightGrow(py::array_t<uint8_t> edge, py::array_t<int> usedMap, py::array_t<float> Des, py::array_t<float>Out, float similarityThreshold, int leastNumofPx, py::array_t<int>_tm, py::array_t<int>_tm2,py::array_t<float>_tm3,int NumOfDims) {
	auto e = edge.mutable_unchecked<3>();
	auto u = usedMap.mutable_unchecked<2>();
	auto o = Out.mutable_unchecked<3>();
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
	float centerOfMass[2] = {0};
	for (int y = 2; y < (e.shape(0) - 3); y+=2) {//skip first line 
		for (int x = 2; x < (e.shape(1) - 3); x+=2) {

			candidateNum[pingpong] = 0;
			candidateNum[!pingpong] = 0;
			bool searchEnd = 0;

			if (u(y, x) ==255) {//should be start of a new region
				//cout << "new search at" << y << "," << x << endl;
				u(y, x) -= 1;//mark as used

				for (int c = 0; c < NumOfDims; c++) {
					avgDes[c] = d(c, y, x);
				}

				m2(pingpong, candidateNum[pingpong], 0 )= y;//candidate[pingpong][candidateNum[pingpong]][0] = y;
				m2(pingpong, candidateNum[pingpong], 1) = x;//candidate[pingpong][candidateNum[pingpong]][1] = x;
				centerOfMass[0] = y;
				centerOfMass[1] = x;
				candidateNum[pingpong]++;
				count = 0;//total count of the region
				m(count,0) = y;
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
									if (u(cy + dy, cx + dx) >0) {//if not used

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
											cosSim+= avgDes[c]* tempDes[c];
										}
										//cout << largestAvgDim<<","<< largestDim << endl;
										int ifSimilar = 0;
										for (int i = largestAvgDim - 2; i <= largestAvgDim + 2; i++) {
											int dim = i % NumOfDims;
											ifSimilar+=(abs(avgDes[dim] - tempDes[dim]) < similarityThreshold);
										}

										if (abs(avgDes[largestAvgDim] - tempDes[largestAvgDim]) <= similarityThreshold && abs(u(cy + dy, cx + dx) - 1 - largestAvgDim) > 5) {//cosSim > similarityThreshold//abs(avgDes[largestAvgDim]- tempDes[largestDim])< similarityThreshold//abs(largestAvgDim - largestDim) < 2
											u(cy + dy, cx + dx) = largestAvgDim+1;//mark as used
											//cout << u(cy + dy, cx + dx) << endl;
											//&& abs(avgDes[secondLarge] - tempDes[secondLarge]) < similarityThreshold 
											//ifSimilar>2 && abs(u(cy + dy, cx + dx)-1- largestAvgDim)>5
											//centerOfMass[1] = (centerOfMass[1] * count + cx + dx) / (count + 1);
											

											m2(!pingpong,candidateNum[!pingpong],0) = cy + dy;//add candidate to the other list
											m2(!pingpong, candidateNum[!pingpong], 1) = cx + dx;
											candidateNum[!pingpong]++;

											//tempGradient[0] *= ((tempGradient[0] * avgGradient[0]) < 0);//inverse
											//tempGradient[1] *= ((tempGradient[1] * avgGradient[1]) < 0);
											for (int c = 0; c < NumOfDims; c++) {//read des
												avgDes[c] = (avgDes[c] * count + tempDes[c]) / (count + 1);//update avg gradient
											}

											if (count < 5000) {
												//cout << count << endl;
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
						float maxLength = 0;
						int r = rand() * 255;
						int g = rand() * 255;
						int b = rand() * 255;
						if (count > leastNumofPx && count <= 5000) {
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
								if ((vy*_dy+vx*_dx) < 0 ) {
									_dy = -_dy;
									_dx = -_dx;
								}
								float length = sqrt(_dy* _dy+ _dx* _dx);
								if (length > maxLength)maxLength = length;
								vy += _dy;// *length;
								vx += _dx;// *length;
							}
							float l2 = sqrt(vy * vy + vx * vx);
							vy /= l2;
							vy *= (maxLength+1);
							vx /= l2;
							vx *= (maxLength+1);
							if (vy < 0) {
								vy = -vy;
								vx = -vx;
							}


							o(lineNum, 0,0) = centerOfMass[0]- vy;
							o(lineNum, 0,1) = centerOfMass[1]- vx;
							o(lineNum, 1,0) = centerOfMass[0]+vy;
							o(lineNum, 1,1) = centerOfMass[1]+vx;
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
	float thresholdT = 0.999;
	float thresholdD = 10;
	for (int time = 0; time < 0; time++) {
		for (int n = 0; n < lineNum; n++) {
			for (int m = 0; m < lineNum; m++) {
				if (n != m) {
					float v1[2] = { o(n,0,0) - o(n,1,0), o(n,0,1) - o(n,1,1) };
					float v2[2] = { o(m,0,0) - o(m,1,0), o(m,0,1) - o(m,1,1) };
					int closest1 = 0;
					int closest2 = 0;
					double sim = CosineSimilarity(v1, v2); 
					if (sim < 0) {
						sim = -sim;
					}
					if (sim > thresholdT) {
						float length = thresholdD* thresholdD;
						for (int p1 = 0; p1 < 2; p1++) {
							for (int p2 = 0; p2 < 2; p2++) {
								float _length = pow(o(n, p1, 0) - o(m, p1, 0), 2) + pow(o(n, p2, 1) - o(m, p2, 1), 2);
								if (_length < length) {
									closest1 = p1;
									closest2 = p2;
									length = _length;
								}
							}
						}

						float v3[2] = { o(n,!closest1,0) - o(m,!closest2,0), o(n,!closest1,1) - o(m,!closest2,1) };
						double sim2 = CosineSimilarity(v1, v3);
						if (sim2 < 0) {
							sim2 = -sim2;
						}
						double sim3 = CosineSimilarity(v2, v3);
						if (sim3 < 0) {
							sim3 = -sim3;
						}
						if ((length != (thresholdD* thresholdD)) && (sim2 > thresholdT) && (sim3 > thresholdT) && ((v3[0] * v3[0] + v3[1] * v3[1]) > 0.8*((v1[0] * v1[0] + v1[1] * v1[1])+(v2[0] * v2[0] + v2[1] * v2[1])) )){//find line to merge
							cout << sim <<endl;
							o(n, closest1, 0) = o(m, !closest2, 0);
							o(n, closest1, 1) = o(m, !closest2, 1);
							o(m, 0, 0) = -9999;
							o(m, 1, 0) = -99999;
							o(m, 0, 1) = -99999;
							o(m, 1, 1) = -9999;
						}
					}
				}
			}
		}
	/*
		int newNum = 0;
		for (int i = 0; i < lineNum; i++) {
			bool merge = 0;
			float v[2] = { o(i, 0,0) - o(i, 1,0),o(i,0, 1) - o(i, 1,1) };

			for (int p = 0; p < lineNum; p++) {
				if (i != p) {
					float _v[2] = { o(p, 0,0) - o(p, 1,0),o(p,0, 1) - o(p, 1,1) };
					if (sqrt((o(i, 1, 0) - o(p, 0, 0)) * (o(i, 1, 0) - o(p, 0, 0)) + (o(i, 1, 1) - o(p, 0, 1)) * (o(i, 1, 1) - o(p, 0, 1))) < 20) {
						if (CosineSimilarity(v, _v) > 0.9) {
							//cout << "merge" << endl;
							float _v[2] = { o(i, 1,0) - o(p,0,0),o(i,1, 1) - o(p, 0,1) };
							if (CosineSimilarity(v, _v) > 0.9) {
								merge = 1;
								_o(newNum, 0, 0) = o(i, 0, 0);
								_o(newNum, 0, 1) = o(i, 0, 1);
								_o(newNum, 1, 0) = o(p, 1, 0);
								_o(newNum, 1, 1) = o(p, 1, 1);
								newNum++;
							}
						}
							
					}
				}
			}
			if (merge == 0) {
				_o(newNum, 0, 0) = o(i, 0, 0);
				_o(newNum, 0, 1) = o(i, 0, 1);
				_o(newNum, 1, 0) = o(i, 1, 0);
				_o(newNum, 1, 1) = o(i, 1, 1);
				newNum++;
			}
		}
		lineNum = 0;
		for (int i = 0; i < newNum; i++) {
			bool merge = 0;
			float v[2] = { _o(i, 0,0) - _o(i, 1,0),_o(i,0, 1) - _o(i, 1,1) };

			for (int p = 0; p < newNum; p++) {
				if (i != p) {
					float _v[2] = { _o(p, 0,0) - _o(p, 1,0),_o(p,0, 1) - _o(p, 1,1) };
					if (sqrt((_o(i, 1, 0) - _o(p, 0, 0)) * (_o(i, 1, 0) - _o(p, 0, 0)) + (_o(i, 1, 1) - _o(p, 0, 1)) * (_o(i, 1, 1) - _o(p, 0, 1))) < 20) {
						if (CosineSimilarity(v, _v) > 0.9) {
							float _v[2] = { _o(i, 1,0) - _o(p,0,0),_o(i,1, 1) - _o(p, 0,1) };
							if (CosineSimilarity(v, _v) > 0.9) {
								merge = 1;
								o(lineNum, 0, 0) = _o(i, 0, 0);
								o(lineNum, 0, 1) = _o(i, 0, 1);
								o(lineNum, 1, 0) = _o(p, 1, 0);
								o(lineNum, 1, 1) = _o(p, 1, 1);
								lineNum++;
							}
						}
							
					}
				}
			}
			if (merge == 0) {
				o(lineNum, 0, 0) = _o(i, 0, 0);
				o(lineNum, 0, 1) = _o(i, 0, 1);
				o(lineNum, 1, 0) = _o(i, 1, 0);
				o(lineNum, 1, 1) = _o(i, 1, 1);
				lineNum++;
			}
		}*/
	}
	
	//cout << "2" << endl;
	return lineNum;
}
int DesGrow1(py::array_t<uint8_t> edge, py::array_t<int> usedMap, py::array_t<float> Des, py::array_t<float>Out, float similarityThreshold, int leastNumofPx, py::array_t<int>_tm, py::array_t<int>_tm2, py::array_t<float>_tm3, int NumOfDims) {
	auto e = edge.mutable_unchecked<3>();
	auto u = usedMap.mutable_unchecked<2>();
	auto o = Out.mutable_unchecked<3>();
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
											u(cy + dy, cx + dx)=0;//mark as used
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
												l2+= avgDes[c]* avgDes[c];
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
						float maxLength = 0;
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
								if ((vy * _dy + vx * _dx) < 0) {
									_dy = -_dy;
									_dx = -_dx;
								}
								float length = sqrt(_dy * _dy + _dx * _dx);
								if (length > maxLength)maxLength = length;
								vy += _dy;// *length;
								vx += _dx;// *length;
							}
							float l2 = sqrt(vy * vy + vx * vx);
							vy /= l2;
							vy *= (maxLength + 1);
							vx /= l2;
							vx *= (maxLength + 1);
							if (vy < 0) {
								vy = -vy;
								vx = -vx;
							}


							o(lineNum, 0, 0) = centerOfMass[0] - vy;
							o(lineNum, 0, 1) = centerOfMass[1] - vx;
							o(lineNum, 1, 0) = centerOfMass[0] + vy;
							o(lineNum, 1, 1) = centerOfMass[1] + vx;
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
	float thresholdT = 0.999;
	float thresholdD = 10;
	for (int time = 0; time < 0; time++) {
		for (int n = 0; n < lineNum; n++) {
			for (int m = 0; m < lineNum; m++) {
				if (n != m) {
					float v1[2] = { o(n,0,0) - o(n,1,0), o(n,0,1) - o(n,1,1) };
					float v2[2] = { o(m,0,0) - o(m,1,0), o(m,0,1) - o(m,1,1) };
					int closest1 = 0;
					int closest2 = 0;
					double sim = CosineSimilarity(v1, v2);
					if (sim < 0) {
						sim = -sim;
					}
					if (sim > thresholdT) {
						float length = thresholdD * thresholdD;
						for (int p1 = 0; p1 < 2; p1++) {
							for (int p2 = 0; p2 < 2; p2++) {
								float _length = pow(o(n, p1, 0) - o(m, p1, 0), 2) + pow(o(n, p2, 1) - o(m, p2, 1), 2);
								if (_length < length) {
									closest1 = p1;
									closest2 = p2;
									length = _length;
								}
							}
						}

						float v3[2] = { o(n,!closest1,0) - o(m,!closest2,0), o(n,!closest1,1) - o(m,!closest2,1) };
						double sim2 = CosineSimilarity(v1, v3);
						if (sim2 < 0) {
							sim2 = -sim2;
						}
						double sim3 = CosineSimilarity(v2, v3);
						if (sim3 < 0) {
							sim3 = -sim3;
						}
						if ((length != (thresholdD * thresholdD)) && (sim2 > thresholdT) && (sim3 > thresholdT) && ((v3[0] * v3[0] + v3[1] * v3[1]) > 0.8 * ((v1[0] * v1[0] + v1[1] * v1[1]) + (v2[0] * v2[0] + v2[1] * v2[1])))) {//find line to merge
							cout << sim << endl;
							o(n, closest1, 0) = o(m, !closest2, 0);
							o(n, closest1, 1) = o(m, !closest2, 1);
							o(m, 0, 0) = -9999;
							o(m, 1, 0) = -99999;
							o(m, 0, 1) = -99999;
							o(m, 1, 1) = -9999;
						}
					}
				}
			}
		}
	}

	//cout << "2" << endl;
	return lineNum;
}


int DesInertiaGrow(py::array_t<uint8_t> edge, py::array_t<int> usedMap, py::array_t<float> Des, py::array_t<float>Out, float similarityThreshold, int leastNumofPx, py::array_t<int>_tm, py::array_t<int>_tm2) {
	auto e = edge.mutable_unchecked<3>();
	auto u = usedMap.mutable_unchecked<2>();
	auto o = Out.mutable_unchecked<2>();
	auto d = Des.mutable_unchecked<3>();
	auto m = _tm.mutable_unchecked<2>();
	auto m2 = _tm2.mutable_unchecked<3>();


	const int NUM_DES = 9;

	int minUnusedNum = 2;
	int center[2] = { 0,0 };
	int count = 0;
	//int candidate[2][300][2] = { 0 };
	float tempDes[NUM_DES] = { 0 };
	bool pingpong = 0;
	float avgDes[NUM_DES] = { 0 };
	int lineNum = 0;
	int ty = 0;
	int tx = 0;
	float centerOfMass[2] = { 0 };
	float inertia[2] = { 0 };
	bool innerCircle[12] = {0};
	float currentVec[2]={ 0 };
	float accVec[2] = { 0 };
	float angleScore = 0;
	float bestVec[2] = { 0 };
	int _y;	int _x;
	float smallestAng = 7.5;
	for (int y = 6; y < (e.shape(0) - 7); y += 1) {//skip first line 
		for (int x = 6; x < (e.shape(1) - 7); x += 1) {
			_y = y;
			_x = x;
			bool searchEnd = 0;
			count = 0;
			//cout << _y << "," << _x << endl;
			if (u(_y, _x) > 0) {//should be start of a new region
				bool firstpixel = 1;
				//cout << "new search at" << y << "," << x << endl;
				for (int c = 0; c < NUM_DES; c++) {//read des
					avgDes[c] = d(c, _y , _x );
				}
				u(_y, _x) -= 1;//mark as used

				while (searchEnd == 0) {
					bool angContinuing = 0;
					float bestScore = 0;
					int endang=-99;
					int startang = -99;
					bool needValidating=0;
					for (int ang = -8; ang < 48; ang++) {//search for a circle of radius 5

						int dx = cos(smallestAng * ang / 180 * PI) * 4.9;
						int dy = sin(smallestAng * ang / 180 * PI) * 4.9;
						//cout <<dx<<","<<dy<< endl;
						float cosSim = 0;
						for (int c = 0; c < NUM_DES; c++) {//read des
							tempDes[c] = d(c, _y + dy, _x + dx);
							cosSim += avgDes[c] * tempDes[c];
						}if (cosSim > similarityThreshold) {//valid searching===============================================================================

						}
						if (u(_y + dy, _x + dx) > 0) {//if valid pixel

							
							//cout << "cos" << cosSim << endl;
							if (angContinuing == 0) {
								//cout << "start" << endl;
								startang = ang;
								angContinuing = 1;
							}
							else {
								endang = ang;
							}
							
						}
						else {
							if (angContinuing) {//part end
								needValidating = 1;
							}
							angContinuing = 0;
						}
						if(needValidating){//if one part search end, start validating

							if (endang != -99) {//valid angle
								int total = 0;
								float valid = 0;
								int _udx;
								int _udy;
								for (int _ang = startang; _ang <= endang; _ang++) {//searching through area under angle range
									for (int r = 0; r < 5; r++) {
										total++;
										_udx = cos(smallestAng * _ang / 180 * PI) * r;
										_udy = sin(smallestAng * _ang / 180 * PI) * r;
										if (u(_y + _udy, _x + _udx) > 0) {
											valid++;
										}
									}
								}
								//cout << "here" << valid << "," << total << endl;
								if ((total>1) && ((valid / total) > 0.3)) {//a valid search angle
									angleScore = ((float)(endang - startang)) / 48 * 0.1;//continuity score
									//cout << " angleScore1" << endang - startang << endl;
									float midAng = (startang + endang) / 2 * 15.0;
									currentVec[0] = sin(midAng / 180 * PI);
									currentVec[1] = cos(midAng / 180 * PI);
									if (!firstpixel) {
										//cout << "cossim: " << CosineSimilarity(currentVec, accVec) << endl;
										angleScore += CosineSimilarity(currentVec, accVec);//angle similarity score
									}

									if (angleScore > bestScore) {//use this angle to proceed
										bestVec[0] = currentVec[0];
										bestVec[1] = currentVec[1];
										bestScore = angleScore;
									}
								}
							}
							if (firstpixel) {
								firstpixel = 0;
								//cout << "next one" << endl;
							}
						}
					}
					if (bestScore != 0) {
						//cout << bestScore << endl;
						for (int dy = -2; dy <= 2; dy++) {
							for (int dx = -2; dx <= 2; dx++) {
								if (u(_y + dy, _x + dx) > 0){
									m(count, 0) = _y + dy;
									m(count, 1) = _x + dx;
									count++;
									u(_y + dy, _x + dx) --;//mark as used one time
								}
							}
						}
						accVec[0] *= 0.5;
						accVec[1] *= 0.5;//decaying
						accVec[0] += bestVec[0];
						accVec[1] += bestVec[1];
						_y += bestVec[0]*2-0.5;
						_x += bestVec[1] * 2-0.5;


						
					}
					else {
						searchEnd = 1;
						//cout << "quit" << endl;
					}
					
				}
				float vy = 0;
				float vx = 0;
				float _dy = 0;
				float _dx = 0;
				float maxLength = 0;
				int r = rand() * 255;
				int g = rand() * 255;
				int b = rand() * 255;
				if (count > leastNumofPx && count < 5000) {
					//cout << count << endl;
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
						if (_dy < 0) {
							_dy = -_dy;
							_dx = -_dx;
						}
						float length = sqrt(_dy * _dy + _dx * _dx);
						if (length > maxLength)maxLength = length;
						vy += _dy * length;
						vx += _dx * length;
					}
					float l2 = sqrt(vy * vy + vx * vx);
					vy /= l2;
					vy *= (maxLength - 2);
					vx /= l2;
					vx *= (maxLength - 2);



					o(lineNum, 0) = centerOfMass[0] - vy;
					o(lineNum, 1) = centerOfMass[1] - vx;
					o(lineNum, 2) = centerOfMass[0] + vy;
					o(lineNum, 3) = centerOfMass[1] + vx;
					lineNum++;
					//cout << "search end with count" << count << endl;
				}

			}



		}
	}
	//cout << "2" << endl;
	return lineNum;
}

int InertiaGrow(py::array_t<uint8_t> edge, py::array_t<int> usedMap, py::array_t<float> Des, py::array_t<float>Out, float similarityThreshold, int leastNumofPx, py::array_t<int>_tm, float intertia_decay) {
	cout << "1" << endl;
	auto e = edge.mutable_unchecked<3>();
	auto u = usedMap.mutable_unchecked<2>();
	auto o = Out.mutable_unchecked<2>();
	auto d = Des.mutable_unchecked<3>();
	auto m = _tm.mutable_unchecked<2>();


	const int NUM_DES = 9;

	int minUnusedNum = 2;
	int center[2] = { 0,0 };
	int count = 0;
	//int candidate[2][300][2] = { 0 };
	float tempDes[NUM_DES] = { 0 };
	bool pingpong = 0;
	float avgDes[NUM_DES] = { 0 };
	int lineNum = 0;
	int ty = 0;
	int tx = 0;
	float centerOfMass[2] = { 0 };
	float inertia[2] = { 0 };
	bool innerCircle[12] = { 0 };
	float currentVec[2] = { 0 };
	float accVec[2] = { 0 };
	float angleScore = 0;
	float bestVec[2] = { 0 };
	int _y;	int _x;
	float pointer[2] = {};
	const int range1 = 2;
	const int range2 = 4;
	const int range3 = 2;
	for (int y = 5; y < (e.shape(0) - 6); y += 1) {//skip first line 
		for (int x = 5; x < (e.shape(1) - 6); x += 1) {
			bool searchEnd = 0;
			//cout << int(1 - 0.7+0.5) << endl;
			//cout << _y << "," << _x << endl;
			if (u(y, x) > 0) {//should be start of a new region
				bool firstpixel = 1;
				count = 0;
				//cout << "new search at" << y << "," << x << endl;
				pointer[0] = y;
				pointer[1] = x;
				accVec[0] = 0;
				accVec[1] = 0;

				u(y, x) -= 1;//mark as used

				while (searchEnd == 0) {
					_y = pointer[0];
					//cout << _y << "," << pointer[0] << endl;
					_x = pointer[1];

					bool angContinuing = 0;
					float bestScore = 0;
					bool needValidating = 0;
					float weightedVec[2] = { 0 };
					accVec[0] *= intertia_decay;//decaying
					accVec[1] *= intertia_decay;
					float dirScore = 0;
					int _count = 0;
					if (firstpixel) {
						for (int dy = -range1; dy <= range1; dy++) {
							for (int dx = -range1; dx <= range1; dx++) {
								if (u(_y + dy, _x + dx) > 0 && dx != 0 && dy != 0) {
									//cout << "first" << _y + dy << "," << _x + dx << endl;

									_count++;
									accVec[0] += dy;
									accVec[1] += dx;
								}
							}
						}
					}
					else {
						//cout << "other" << endl;
						for (int dy = -range2; dy <= range2; dy++) {
							for (int dx = -range2; dx <= range2; dx++) {
								if (u(_y + dy, _x + dx) > 0 && dx != 0 && dy != 0) {
									//cout << "other" << _y + dy << "," << _x + dx << endl;
									float _vec[2] = { dy,dx };
									float weight = CosineSimilarity(accVec, _vec);
									if (weight > 0.65) {
										_count++;
										dirScore += weight;
										accVec[0] += dy * weight;
										accVec[1] += dx * weight;
									}
								}
							}
						}
						dirScore /= _count;
					}
					//cout << accVec[0] << "," << accVec[1] << endl;

					//cout << dirScore << endl;
					float intensity = sqrt(accVec[0] * accVec[0] + accVec[1] * accVec[1]);
					if ((dirScore > 0.5 || firstpixel) &&_count>0&& (accVec[0]!=0|| accVec[1] != 0)&& intensity>3) {//next round
						for (int dy = -range3; dy <= range3; dy++) {
							for (int dx = -range3; dx <= range3; dx++) {
								if (u(_y + dy, _x + dx) > 0) {
									m(count, 0) = _y + dy;
									m(count, 1) = _x + dx;
									count++;
									u(_y + dy, _x + dx)--;//mark as used one time
								}
							}
						}
						//cout << "acc" << accVec[0] << "," << accVec[1] << endl;
						//cout << "count" << count << endl;
						float* nextDir = normalize(accVec[0], accVec[1]);
						//cout << "next"<<nextDir[0] << "," << nextDir[1] << endl;
						pointer[0] += nextDir[0]/2;
						pointer[1] += nextDir[1]/2;
						if(pointer[0]<6|| pointer[0]>(u.shape(0)-6)|| pointer[1]<6 || pointer[0]>(u.shape(1)-6))	searchEnd = 1;
					}
					else {
						searchEnd = 1;
						//cout << "quit" << endl;
					}
					firstpixel = 0;

				}
				float vy = 0;
				float vx = 0;
				float _dy = 0;
				float _dx = 0;
				float maxLength = 0;
				int r = rand() * 255;
				int g = rand() * 255;
				int b = rand() * 255;
				if (count > leastNumofPx && count < 5000) {
					//cout << count << endl;
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
						if (_dy < 0) {
							_dy = -_dy;
							_dx = -_dx;
						}
						float length = sqrt(_dy * _dy + _dx * _dx);
						if (length > maxLength)maxLength = length;
						vy += _dy * length;
						vx += _dx * length;
					}
					float l2 = sqrt(vy * vy + vx * vx);
					vy /= l2;
					vy *= (maxLength - 2);
					vx /= l2;
					vx *= (maxLength - 2);



					o(lineNum, 0) = centerOfMass[0] - vy;
					o(lineNum, 1) = centerOfMass[1] - vx;
					o(lineNum, 2) = centerOfMass[0] + vy;
					o(lineNum, 3) = centerOfMass[1] + vx;
					centerOfMass[1] = 0;
					centerOfMass[0] = 0;
					lineNum++;
					//cout << "search end with count" << count << endl;
				}

			}



		}
	}
	//cout << "2" << endl;
	return lineNum;
}


int localMax(py::array_t<float_t> in, py::array_t<int> out,float threshold) {
	auto r = in.mutable_unchecked<3>();
	auto outarr = out.mutable_unchecked<3>();
	int Pcount = 0;
	int minUnusedNum = 2;
	int center[2] = { 0,0 };
	int startLine[256] = { 0 };
	int regionCount = 0;
	int ty = 0;
	int tx = 0;
	int regionCout = 0;
	int tobeChanged = 0;
	int newNum = 0;
	float _v;
	float _v2;
	float _m;
	int y;
	int x;
	for (int __y = 7; __y < (r.shape(1) - 7); __y += 2) {//scan every 2x2 pixel
		for (int __x = 7; __x < (r.shape(2) - 7); __x += 2) {
			//cout << "start at " << __x << "," << __y << "value is!!!!!!!!!!!!!!!!!!!!!!" << _v << endl;
			y = __y;
			x = __x;
			//cout << "here2" << endl;
			if (r(0, y, x) >= threshold) {
				//cout << "here3" << endl;
			startAgain:
				_v = r(0, y, x);
				//cout << "start at " << x << "," << y << "value is" << _v << endl;
				_m = _v;
				bool notMaximum = 0;
				for (int yy = -2; yy <= 2; yy++) {
					for (int xx = -2; xx <= 2; xx++) {
						if (yy != 0 && xx != 0) {
							//cout << "here" << endl;
							_v2 = r(0, y + yy, x + xx);
							if (_v2 > _m) {
								_m = _v2;//larger value detected nearby
								//r(batch, y, x) = 0;//mark pixel as used
								notMaximum = 1;
								x = x + xx;
								y = y + yy;
								if (x > 0 && x < r.shape(2) && y>0 && y < r.shape(1)) {
									//cout << "proceed"<<endl;
									goto startAgain;
								}
							}
						}
					}
				}
				bool duplicated = 0;
				bool equal = 0;
				int ed = 0;
				for (int d = 0; d < Pcount; d++) {
					if (outarr(0, d, 0) == y && outarr(0, d, 1) == x)duplicated = 1;
					if (abs(outarr(0, d, 0) - y) <= 1 && abs(outarr(0, d, 1) - x) <= 1) {
						//cout << "equal";
						ed = d;
						equal = 1;
					}
				}
				if (duplicated == 0 && equal ==0) {
					outarr(0,Pcount, 0) = y;
					outarr(0,Pcount, 1) = x;
					Pcount++;
					//cout << "max is "<<_m<<"at"<<y<<","<<x<<"\n";
				}
				else if (duplicated == 0 && equal == 1) {
					outarr(0, ed, 0) += y;
					outarr(0, ed, 0) /=2;
					outarr(0, ed, 1) += x;
					outarr(0, ed, 1) /=2;
					//cout << "max is "<<_m<<"at"<<y<<","<<x<<"\n";
				}

				


				//found a unused pixel
			}
		}
	}
	
	return Pcount;
}

PYBIND11_MODULE(extractC2, m) {
	//py::register_exception<std::exception>(m, "PyExp");
	m.doc() = "pybind11 example plugin"; // optional Bmodule docstring

	m.def("add", &add, "A function that adds two numbers");
	m.def("f1", &f1, "A function that adds two numbers");
	m.def("extractPoint", &extract, "extract center mass point");
	m.def("seg", &segmentation, "extract center mass point");
	m.def("extractPoint2", &scan, "extract center mass point");
	m.def("findLocalMax", &localMax, "extract center mass point");
	m.def("gradientGrow", &gradientGrow, "extract center mass point");
	m.def("desGrow", &DesGrow1, "extract center mass point");
	m.def("desGrow2", &straightGrow, "extract center mass point");
	m.def("desInertiaGrow", &DesInertiaGrow, "extract center mass point");
	m.def("inertiaGrow", &InertiaGrow, "extract center mass point");


}