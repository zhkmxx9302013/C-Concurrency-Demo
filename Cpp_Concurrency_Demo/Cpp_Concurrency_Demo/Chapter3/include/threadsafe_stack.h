#pragma once
/************************************************************************/
/* �����ӿڿ��Ի�����̶ȵİ�ȫ,�������ƶ�ջ��һЩ������
 * ջ�ǲ���ֱ�Ӹ�ֵ�ģ���ֵ�����Ѿ�ɾ��
 * û��swap()������
 * ջ���Կ����ģ�����ջ�е�Ԫ�ؿ��Կ�������ջΪ��ʱ��pop()�������׳�һ��empty_stack�쳣��������empty()���������ú�����������������������
 * ��ѡ��3������������ʹ�� std::shared_ptr ���Ա����ڴ�����������⣬��������ʹ��new��delete������
 * ��ջ�е�������������ھ�ʣ��������push(), pop()��empty()(����empty()����Щ����)��
 * �򻯽ӿڸ����������ݿ��ƣ����Ա�֤��������һ��������ȫ��ס��                                                                     */
/************************************************************************/
#include <exception> 
#include <memory>
#include <mutex>
#include <stack>

struct empty_stack: std::exception 
{
	const char* what() const throw() 
	{
		return "This is a empty stack";
	}
};


template<typename T>
class threadsafe_stack
{
public:
	threadsafe_stack() : data(std::stack<T>()){}

	//���ṩ�ڿ�����������ִ�п���
	threadsafe_stack(const threadsafe_stack& other)
	{
		std::lock_guard<std::mutex> lock(other.m);
		data = other.data;	
	}

	threadsafe_stack& operator=(const threadsafe_stack&) = delete;
	~threadsafe_stack();

	void push(T new_value)
	{
		std::lock_guard<std::mutex> lock(m);
		data.push(new_value);
	}

	// ����3��popʵ��
	std::shared_ptr<T> pop()
	{
		std::lock_guard<std::mutex> lock(m);
		if (data.empty()) throw empty_stack();

		//�ڳ�ջǰ�ȷ��乲��̬�ڴ棬������ջֵ
		std::shared_ptr<T> const pop_result_ptr(std::make_shared<T>(data.top()));

		//��ջ
		data.pop();

		//Return
		return pop_result_ptr;
	}

	//����1 ��popʵ��
	void pop(T& value)
	{
		std::lock_guard<std::mutex> lock(m);
		if (data.empty()) throw empty_stack();

		//�ڳ�ջǰ������ջֵ������
		value = data.top();
		data.pop();
	}


	bool empty() const
	{
		std::lock_guard<std::mutex> lock(m);
		return data.empty();
	}

private:
	std::stack<T> data;
	mutable std::mutex m;
};

