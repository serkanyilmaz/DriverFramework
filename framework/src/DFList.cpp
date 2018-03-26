#include "DFList.hpp"

using namespace DriverFramework;

DFPointerList::~DFPointerList()
{
	m_sync.lock();
	Index next_del = m_head;

	while (next_del != nullptr) {
		Index curr = next_del;
		next_del = curr->m_next;
		delete curr;
	}

	m_head = nullptr;
	m_sync.unlock();
}

unsigned int DFPointerList::size()
{
	m_sync.lock();
	unsigned int rv = m_size;
	m_sync.unlock();
	return rv;
}

bool DFPointerList::pushBack(void *item)
{
	m_sync.lock();
	Index t = new DFListNode(item);

	if (t == nullptr) {
		m_sync.unlock();
		return false;
	}

	if (m_head == nullptr) {
		m_head = t;
		m_end = t;

	} else {
		m_end->m_next = t;
		m_end = t;
	}

	++m_size;
	m_sync.unlock();
	return true;
}

bool DFPointerList::pushFront(void *item)
{
	m_sync.lock();
	Index t = new DFListNode(item);

	if (t == nullptr) {
		m_sync.unlock();
		return false;
	}

	if (m_head == nullptr) {
		m_head = t;
		m_end = t;

	} else {
		t->m_next = m_head;
		m_head = t;
	}

	++m_size;
	m_sync.unlock();
	return true;
}

DFPointerList::Index DFPointerList::erase(Index idx)
{
	m_sync.lock();

	if (idx != nullptr && idx == m_head) {
		Index next_erase = m_head->m_next;
		delete m_head;
		m_head = next_erase;

		if (next_erase == nullptr) {
			m_end = nullptr;
		}

		--m_size;
		m_sync.unlock();
		return m_head;

	} else {
		for (Index curr = m_head; curr->m_next != nullptr;) {
			Index next_erase = curr->m_next;

			if (idx == next_erase) {
				curr->m_next = next_erase->m_next;

				if (next_erase == m_end) {
					m_end = curr;
				}

				delete next_erase;
				--m_size;
				m_sync.unlock();
				return curr->m_next;
			}

			curr = next_erase;
		}
	}

	m_sync.unlock();
	return nullptr;
}

void DFPointerList::clear()
{
	m_sync.lock();

	while (m_head != nullptr) {
		Index tmp = m_head;
		m_head = m_head->m_next;
		delete tmp;
	}

	m_sync.unlock();
}

bool DFPointerList::empty()
{
	m_sync.lock();
	bool is_empty = (m_head == nullptr);
	m_sync.unlock();
	return is_empty;
}

DFPointerList::Index DFPointerList::next(Index &idx)
{
	m_sync.lock();

	if (idx == nullptr) {
		idx = m_head;

	} else {
		idx = idx->m_next;
	}

	m_sync.unlock();
	return idx;

}

void *DFPointerList::get(Index idx)
{
	if (idx) {
		return idx->m_item;
	}

	return nullptr;
}

DFUIntList::~DFUIntList()
{
	m_sync.lock();
	Index next_del = m_head;

	while (next_del != nullptr) {
		Index curr = next_del;
		next_del = curr->m_next;
		delete curr;
	}

	m_head = nullptr;
	m_sync.unlock();
}

unsigned int DFUIntList::size()
{
	return m_size;
}

bool DFUIntList::pushBack(unsigned int item)
{
	m_sync.lock();
	Index t = new DFUIntListNode(item);

	if (t == nullptr) {
		m_sync.unlock();
		return false;
	}

	if (m_head == nullptr) {
		m_head = t;
		m_end = t;

	} else {
		m_end->m_next = t;
		m_end = t;
	}

	++m_size;
	m_sync.unlock();
	return true;
}

bool DFUIntList::pushFront(unsigned int item)
{
	m_sync.lock();
	Index t = new DFUIntListNode(item);

	if (t == nullptr) {
		m_sync.unlock();
		return false;
	}

	if (m_head == nullptr) {
		m_head = t;
		m_end = t;

	} else {
		t->m_next = m_head;
		m_head = t;
	}

	++m_size;
	m_sync.unlock();
	return true;
}

DFUIntList::Index DFUIntList::erase(Index idx)
{
	m_sync.lock();

	if (idx != nullptr && idx == m_head) {
		Index t = m_head->m_next;
		delete (m_head);
		m_head = t;

		if (t == nullptr) {
			m_end = nullptr;
		}

		--m_size;
		m_sync.unlock();
		return m_head;

	} else {
		for (Index p = m_head; p->m_next != nullptr;) {
			Index t = p->m_next;

			if (idx == t) {
				p->m_next = t->m_next;

				if (t == m_end) {
					m_end = p;
				}

				delete (t);
				--m_size;
				m_sync.unlock();
				return p->m_next;
			}

			p = t;
		}
	}

	m_sync.unlock();
	return nullptr;
}

void DFUIntList::clear()
{
	while (m_head != nullptr) {
		erase(m_head);
	}
}

bool DFUIntList::empty()
{
	m_sync.lock();
	bool is_empty = (m_head == nullptr);
	m_sync.unlock();
	return is_empty;
}

DFUIntList::Index DFUIntList::next(Index &idx)
{
	m_sync.lock();

	if (idx == nullptr) {
		idx = m_head;

	} else {
		idx = idx->m_next;
	}

	m_sync.unlock();
	return idx;
}

bool DFUIntList::get(Index idx, unsigned int &val)
{
	if (idx) {
		val = idx->m_item;
		return true;
	}

	return false;
}

