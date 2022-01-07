#include <iostream>
#include <opencv2/opencv.hpp>
using namespace std;
using namespace cv;


// 冒泡排序
void Sort(int* a, int n)		//把数组a递增排序
{
	//for (int gap = n / 2; gap > 0; gap /= 2)//希尔排序
	//	for (int i = gap; i < n; ++i)
	//		for (int j = i - gap; j >= 0 && a[j] > a[j + gap]; j -= gap)
	//			swap(a[j], a[j + gap]);

	for (int i = 0; i < n; i++) {
		for (int j = i + 1; j < n; j++)
		{
			if (a[i] > a[j]) {
				int tmp = a[i];
				a[i] = a[j];
				a[j] = tmp;
			}
		}
	}

}

// 中值滤波
void MedianFilter(cv::Mat &srcImg, cv::Mat& dstImg, int ksize)
{
	// 判断原图像是否为空
	if (srcImg.empty()) {
		return;
	}
	// 判断核的大小是否为奇数
	CV_Assert(ksize % 2 == 1);

	// 获取通道数
	int channels = srcImg.channels();

	// 清空目标图像，对原图像进行边界填充
	Mat tmp;
	dstImg = dstImg.zeros(srcImg.size(), srcImg.type());
	int *kernel = new int[ksize*ksize];
	copyMakeBorder(srcImg, tmp, ksize / 2, ksize / 2, ksize / 2, ksize / 2, BORDER_REPLICATE);
	for (int i = ksize / 2; i < srcImg.rows + ksize / 2; i++)		//对填充后的图像从有图像区域开始滤波
	{
		for (int j = ksize / 2; j < srcImg.cols + ksize / 2; j++)
		{
			for (int c = 0; c < channels; c++)
			{
				// 将核大小的图像填入数组
				for (int m = 0; m < ksize*ksize; m++) {
					if (tmp.channels() == 1) {
						kernel[m] = tmp.ptr<uchar>(i - ksize / 2 + m / ksize, j - ksize / 2 + m % ksize)[c];
					}
					else if (tmp.channels() == 3) {
						kernel[m] = tmp.ptr<Vec3b>(i - ksize / 2 + m / ksize, j - ksize / 2 + m % ksize)->val[c];
					}
					else
					{
						delete[]kernel;
						return;
					}

				}

				// 排序	
				Sort(kernel, ksize*ksize);
				// 将中值写入目标图像
				if (tmp.channels() == 1) {
					dstImg.ptr<uchar>(i - ksize / 2, j - ksize / 2)[c] = kernel[(ksize*ksize) / 2];
				}
				else if (tmp.channels() == 3) {
					dstImg.ptr<Vec3b>(i - ksize / 2, j - ksize / 2)->val[c] = kernel[(ksize*ksize) / 2];
				}

			}
		}
	}
	delete[]kernel;

}


int main() {
	Mat image = imread("C:\\Users\\86185\\Desktop\\OIP-C(13).jpg");
	if (image.empty()) {
		return -1;
	}


	Mat image2, image3;
	double t1 = (double)cv::getTickCount() / 1000;
	MedianFilter(image, image2, 3);

	imshow("原图", image);
	imshow("自定义中值滤波处理后", image2);



	waitKey(0);
	return 0;
}
