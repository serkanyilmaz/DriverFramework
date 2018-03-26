#pragma once

#include "DisableCopy.hpp"
#include "SyncObj.hpp"

namespace DriverFramework
{


class DFPointerList : public DisableCopy
{
public:
	class DFListNode;

	typedef DFListNode *Index;

	class DFListNode
	{
	public:
		DFListNode(void *item) : m_item(item) {}
		~DFListNode() = default;

		Index	m_next{nullptr};
		void 	*m_item;
	};

	DFPointerList() = default;
	virtual ~DFPointerList();

	unsigned int size();

	bool pushBack(void *item);

	bool pushFront(void *item);

	virtual Index erase(Index idx);

	virtual void clear();

	bool empty();

	Index next(Index &idx);

	void *get(Index idx);

	// Caller must lock before using list
	SyncObj m_sync;

private:

	Index		m_head{nullptr};
	Index		m_end{nullptr};
	unsigned int 	m_size{0};
};

template <class T>
class DFManagedList : public DFPointerList
{
public:
	DFManagedList() : DFPointerList() {}

	virtual ~DFManagedList()
	{
		Index idx = nullptr;
		idx = next(idx);

		while (idx != nullptr) {
			T *tmp = get(idx);
			delete tmp;
			idx = next(idx);
		}
	}

	T *get(Index idx)
	{
		return static_cast<T *>(DFPointerList::get(idx));
	}

	virtual Index erase(Index idx)
	{
		if (idx != nullptr) {
			T *tmp = get(idx);
			delete tmp;
			return DFPointerList::erase(idx);
		}

		return idx;
	}

	virtual void clear()
	{
		Index idx = nullptr;
		idx = next(idx);

		while (idx != nullptr) {
			T *tmp = get(idx);
			delete tmp;
			idx = next(idx);
		}

		DFPointerList::clear();
	}

	bool pushBack(T *item)
	{
		return DFPointerList::pushBack((void *)item);
	}

	bool pushFront(T *item)
	{
		return DFPointerList::pushFront((void *)item);
	}
};

class DFUIntList : public DisableCopy
{
public:
	class DFUIntListNode;

	typedef DFUIntListNode *Index;

	class DFUIntListNode
	{
	public:
		DFUIntListNode(unsigned int item) : m_item(item) {}
		~DFUIntListNode() = default;

		Index		m_next{nullptr};
		unsigned int	m_item;
	};

	DFUIntList() = default;
	~DFUIntList();

	unsigned int size();

	bool pushBack(unsigned int item);

	bool pushFront(unsigned int item);

	Index erase(Index idx);

	void clear();

	bool empty();

	Index next(Index &idx);

	bool get(Index idx, unsigned int &val);

	// Caller must lock before using list
	SyncObj m_sync;

private:

	Index		m_head{nullptr};
	Index		m_end{nullptr};
	unsigned int 	m_size{0};
};

}
