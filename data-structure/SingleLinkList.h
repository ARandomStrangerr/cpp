#ifndef SINGLE_LINK_LIST
#define SINGLE_LINK_LIST

#include <cstdio>
#include <ostream>
#include <stdexcept>
#include <iostream>
#include <utility>

template<typename T> class Node;
template<typename T> class SingleLinkList;

template<typename T> class Node {
	friend class SingleLinkList<T>;
	template<typename U> friend std::ostream& operator<< (std::ostream& os, const SingleLinkList<U>& list);
	private:
		T data;
		Node<T>* next;
		Node(const T& data) : data(data), next(nullptr){}
	public:	
};

template<typename T> class SingleLinkList {
	private: 
		Node<T>* head;
		int size;
	public:
		SingleLinkList() : head(nullptr), size(0){}
		/*
		 * @brief: return the size of the structure
		 */
		int length(){
			return size;	
		}
		/*
		 *  @brief: insert an element infront of the list
		 */
		void insertFirst(const T& data){
			Node<T>* node = new Node<T>(data);
			if (size != 0) node -> next = head;
			head = node;
			size++;
		}
		/*
		 * @brief: insert an element at the end of the list
		 */
		void insertLast(const T& data){
			Node<T>* node = new Node<T>(data);
			if (size != 0) {
				Node<T>* tail = head;
				for (; tail->next != nullptr; tail=tail->next);
				tail->next = node;
			} else head = node;
			size++;
		}
		/*
		 * @brief: insert an element into arbitrary position of the list
		 */
		void insert(const T& data, int position){
			if (position > 0 && position < size){
				Node<T>* node = new Node<T>(data);
				Node<T>* prevInsertPosition = head;
				for (int i = 0; i < position-1; i++, prevInsertPosition=prevInsertPosition->next);
				node->next = prevInsertPosition->next; 
				prevInsertPosition->next = node;
				size++;
			} else if (position == 0) insertFirst(data);
			else if (position == size - 1) insertLast(data);
			else throw std::out_of_range("position index is out of range");
		}

		/*
		 * @breif: get the first element of the list
		 */
		T& getFirst() {
			if (head == nullptr) throw std::length_error("there is no item in this collection");
			return head->data;
		}

		/*
		 * @breif: get the last element of the list
		 */
		T& getLast() {
			Node<T>* tail = head;
			for (;tail->next != nullptr; tail=tail->next);
			return tail->data;
		}

		/*
		 * @brief: get an element at arbitrary position
		 */
		T& get(int position){
			if (position > 0 && position < size-1){
				Node<T>* node = head;
				for (int i=0; i != position; i++, node = node->next);
				return node->data;
			} else if (position == 0) return getFirst();
			else if (position == size-1) return getLast();
			else if (head == nullptr) throw std::length_error("there is no item in this collection");
			else throw std::out_of_range("position index is out of range");
		}

		T removeFirst(){
			if (head == nullptr) throw std::length_error("there is no item in this collection");
			Node<T>* removeNode = head;
			head = head->next;
			T returnData = std::move(removeNode->data); 
			delete removeNode;
			size--;
			return returnData;
		}
		
		T removeLast() {
			if (head == nullptr) throw std::length_error("there is no item in this collection");
			else if (head->next == nullptr) return removeFirst();
			Node<T>* tail = head;
			Node<T>* prev = nullptr;
			for(; tail->next != nullptr; prev = tail, tail = tail->next);
			T returnData = std::move(tail->data);
			prev->next = nullptr;
			delete tail;
			size--;
			return returnData;
		}

		T remove(int index) {
			if (index > 0 && index < size-1){
				Node<T>* removeNode = head;
				Node<T>* prev = nullptr;
				for (int i = 0; i < index; i++, prev = removeNode, removeNode = removeNode->next);
				T returnData = std::move(removeNode->data);
				prev->next = removeNode->next;
				delete removeNode;
				size--;
				return returnData;
			} else if (index == 0) return removeFirst();
			else if (index == size-1) return removeLast();
			else throw std::out_of_range("given position index is out of range");
		}

		template<typename U> friend std::ostream& operator<< (std::ostream&, const SingleLinkList<U>&);
};
template<typename T> std::ostream& operator<< (std::ostream& os, const SingleLinkList<T>& list){
	os << "[ ";
	for (Node<T>* node = list.head; node!=nullptr; node = node->next) os << node -> data << " ";
	os << "]";
	return os;
}
#endif
