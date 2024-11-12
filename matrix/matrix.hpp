#ifndef MATRIX_HPP
#define MATRIX_HPP

#include <iostream>
#include <cstring>
#include <type_traits>
#include <cmath>

namespace math
{
	template<typename T>
	class Matrix
	{
		template<typename P = T>
		struct M_Ptr
		{
			int rows_;
			int cols_;
			P **data_;

			void allocCols(P **data_m, int rowInd)
			{
				try	{ data_m[rowInd] = new P[cols_]{};	}
				catch(...)
				{
					for(int j = 0; j < rowInd; ++j)
						delete[] data_m[j];

					delete[] data_m;
					throw;
				}
			}

			M_Ptr(int rows, int cols) : rows_{rows}, cols_{cols}
			{
				P **data_tmp = new P*[rows_]{};

				for(int i = 0; i < rows_; ++i)
					allocCols(data_tmp, i);

				data_ = data_tmp;
			};

			M_Ptr(const M_Ptr& other) : rows_{other.rows_}, cols_{other.cols_}
			{
				P **data_tmp = new P*[rows_]{};

				for(int i = 0; i < rows_; ++i)
				{
					allocCols(data_tmp, i);
					std::memcpy(data_tmp[i], other.data_[i], cols_*sizeof(P));
				}

				data_ = data_tmp;
			};

			M_Ptr(M_Ptr&& other) noexcept : rows_{other.rows_}, cols_{other.cols_}, data_{other.data_} { other.data_ = nullptr; };

			M_Ptr& operator=(const M_Ptr& other)
			{
				M_Ptr tmp(other);
				swap(tmp);

				return *this;
			}

			M_Ptr& operator=(M_Ptr&& other) noexcept
			{
				if(this == &other)
					return *this;
				
				swap(other);

				return *this;
			}

			void deleteM_Ptr()
			{
				if(data_)
					for(int i = 0; i < rows_; ++i)
						delete[] data_[i];

				delete[] data_;
				data_ = nullptr;
			}

			~M_Ptr() { deleteM_Ptr(); }

			void swap(M_Ptr& other) noexcept
			{
				std::swap(rows_, other.rows_);
				std::swap(cols_, other.cols_);
				std::swap(data_, other.data_);
			}
		};

		M_Ptr<> m_Ptr;

		class M_Row
		{
			T *data_;

		public:
			M_Row(T* data) : data_{data} {};
			T& operator[](int j) { return data_[j]; }
		};

	public:
		Matrix(int rows = 1, int cols = 1) : m_Ptr{rows, cols}
		{
			try	{ checkType(); }
			catch(char const * errM)
			{
				m_Ptr.deleteM_Ptr();
				std::cerr << errM << '\n';
				throw;
			}
		};
		const int rows() const { return m_Ptr.rows_; };
		const int cols() const { return m_Ptr.cols_; };
		M_Row operator[](int i) { return M_Row{m_Ptr.data_[i]}; }
		T determinant();
	
	private:
		void checkType();

		template<typename D>
		D compDeterminant(M_Ptr<D>& ptr);
		struct indexesNode { int vI = 0, hI = 0; };
		template<typename D>
		indexesNode maxValIndexes(M_Ptr<D>& ptr, int sI);
		template<typename D>
		void subtractRows(M_Ptr<D>& ptr, int sI);
	};
};

template <typename T>
void math::Matrix<T>::checkType()
{
	if(std::is_fundamental_v<T>)
		return;
	
	throw "\nBad Type of Matrix\n";
}

template <typename T>
T math::Matrix<T>::determinant()
{
	/*what's do with unsigned???*/
	if(m_Ptr.rows_ != m_Ptr.cols_)
		throw "\nFor computing determinant should rows == columns\n";
	
	if(m_Ptr.rows_ == 1 && m_Ptr.cols_ == 1)
		return m_Ptr.data_[0][0];

	T det = T{};

	if(std::is_floating_point_v<T>)
	{
		M_Ptr<> mP(m_Ptr);
		return compDeterminant(mP);
	}

	M_Ptr<double> mP{m_Ptr.rows_, m_Ptr.cols_};
	for(int i = 0; i < m_Ptr.rows_; ++i)
		for(int j = 0; j < m_Ptr.cols_; ++j)
			mP.data_[i][j] = static_cast<double>(m_Ptr.data_[i][j]);

	return static_cast<T>(std::round(compDeterminant(mP)));
}

template <typename T>
template <typename D>
D math::Matrix<T>::compDeterminant(M_Ptr<D>& ptr)
{
	int lim = ptr.rows_ - 1;
	D det = 1;

	for(int i = 0; i < lim; ++i)
	{
		indexesNode maxNode = maxValIndexes(ptr, i);

		if(!ptr.data_[maxNode.vI][maxNode.hI])
			return 0;

		if(i != maxNode.vI)
		{
			std::swap(ptr.data_[i], ptr.data_[maxNode.vI]);
			det = -det;
		}
		if(i != maxNode.hI)
		{
			for(int j = 0; j < ptr.rows_; ++j)
				std::swap(ptr.data_[j][i], ptr.data_[j][maxNode.hI]);

			det = -det;
		}

		subtractRows(ptr, i);
	}

	for(int i = 0; i < ptr.rows_; ++i)
		det *= ptr.data_[i][i];

	return det;
}

template <typename T>
template <typename D>
typename math::Matrix<T>::indexesNode math::Matrix<T>::maxValIndexes(M_Ptr<D> &ptr, int sI)
{
	indexesNode iN{ sI, sI };

	D max = std::fabs(ptr.data_[sI][sI]);
	const int vSize = ptr.cols_, hSize = ptr.rows_;
	for(int i = sI; i < vSize; ++i)
		for(int j = sI; j < hSize; ++j)
			if(max < std::fabs(ptr.data_[i][j]))
			{
				max = std::fabs(ptr.data_[i][j]);
				iN.vI = i;
				iN.hI = j;
			}

	return iN;
}

template <typename T>
template <typename D>
void math::Matrix<T>::subtractRows(M_Ptr<D> &ptr, int sI)
{
	for(int i = sI + 1, j = sI; i < ptr.rows_; ++i, j = sI)
	{
		D rat = ptr.data_[i][j] / ptr.data_[sI][j];
		ptr.data_[i][j] = 0;

		for(++j; j < ptr.cols_; ++j)
			ptr.data_[i][j] -= rat * ptr.data_[sI][j];
	}
}

#endif