#pragma once

#include <iostream>
#include <vector>

namespace mat {
	class BaseMat {
		public:
			BaseMat(int lns, int cols);  // init with zero matrix
			BaseMat(int size);  // init with identity matrix
			BaseMat(int lns, int cols, std::vector<float> data);  // init with a copy of data
			BaseMat(const BaseMat &m);
			~BaseMat();

			void setLns(int lns);  // set number of lines
			void setCols(int cols);  // set number of columns
			int getLns() const;  // get number of lines
			int getCols() const;  // get number of columns
			const std::vector<float> &getData() const;  // get all data as a vector
			std::vector<float> &getData();
			float &get(int ln, int col);  // get one data (line, column)
			const float &get(int ln, int col) const;
			std::string getPrintableNb(float nb) const;  // get a printable number (for cout)

			/* operators */
			friend std::ostream &operator<<(std::ostream &out, const BaseMat &m);
			friend BaseMat operator*(BaseMat &m, const float other);
			friend BaseMat operator*(BaseMat &m, const BaseMat &other);
			friend BaseMat operator+(BaseMat &m, const float other);
			friend BaseMat operator+(BaseMat &m, const BaseMat &other);
			friend BaseMat operator-(BaseMat &m, const float other);
			friend BaseMat operator-(BaseMat &m, const BaseMat &other);
			BaseMat &operator=(const BaseMat &other);
		protected:
			int _lns;  // number of lines (height)
			int _cols;  // number of columns (width)
			/*
			for the Matrix:
			| 1 2 |
			| 3 4 |
			float *data = {1, 2, 3, 4};
			*/
			std::vector<float> *_data;  // all data
		private:
	};

	class Vec : public BaseMat {
		public:
			Vec(int size);
			Vec(int size, std::vector<float> data);
			~Vec();

			int getSize() const;
			const float &get(int x) const;
			float &get(int x);
			const float &operator[](const int idx) const;
			float &operator[](const int idx);

			/*
			xyzw and rgb are 'shortcut' to acces to the datas
			*/
			float &x;
			float &y;
			float &z;
			float &w;
			float &r;
			float &g;
			float &b;
		protected:
		private:
	};
	class Vec2 : public Vec {
		public:
			Vec2();  // 0 0
			Vec2(std::vector<float> data);
			~Vec2();
		protected:
		private:
	};
	class Vec3 : public Vec {
		public:
			Vec3();  // 0 0 0
			Vec3(std::vector<float> data);
			~Vec3();
		protected:
		private:
	};
	class Vec4 : public Vec {
		public:
			Vec4();  // 0 0 0 1
			Vec4(std::vector<float> data);
			~Vec4();
		protected:
		private:
	};

	class SquareMat : public BaseMat {
		public:
			SquareMat(int size, bool identity=true);
			SquareMat(int size, std::vector<float> data);
			~SquareMat();

			int getSize() const;
		protected:
		private:
	};
	class Mat2 : public SquareMat {
		public:
			Mat2(bool identity=true);
			Mat2(std::vector<float> data);
			~Mat2();
		protected:
		private:
	};
	class Mat3 : public SquareMat {
		public:
			Mat3(bool identity=true);
			Mat3(std::vector<float> data);
			~Mat3();
		protected:
		private:
	};
	class Mat4 : public SquareMat {
		public:
			Mat4(bool identity=true);
			Mat4(std::vector<float> data);
			~Mat4();

			Mat4 scale(Vec3 val);  // scale martix
			Mat4 scale(float val);  // scale martix
			Mat4 scale(float valX, float valY, float valZ);  // scale martix

			Mat4 translate(Vec3 val);  // translate martix
			Mat4 translate(float valX, float valY, float valZ);  // translate martix

			Mat4 rotateRad(float radians, Vec3 axis);  // rotate around axis
			Mat4 rotateRad(float radians, float axX, float axY, float axZ);  // rotate around axis
			Mat4 rotateDeg(float degrees, Vec3 axis);  // rotate around axis
			Mat4 rotateDeg(float degrees, float axX, float axY, float axZ);  // rotate around axis
		protected:
		private:
	};
}