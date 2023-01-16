#ifndef RINGBUFFER_HPP
#define RINGBUFFER_HPP

#include <cstdint>



template <typename T>
class Ringbuffer
{
private:
    T* buffer;
    uint16_t head, tail;
    uint16_t length;

    

public:
    Ringbuffer();
    Ringbuffer(T* buffer_ptr, uint16_t buff_length);
    
    ~Ringbuffer();

    uint16_t put(T data);
    uint16_t put(T *data, uint16_t size);

    T pop();

    uint16_t storage_available() const;
    bool is_full() const;
    bool is_empty() const;   
    uint16_t in_storage() const;

    void clear_buffer();
};


template <typename T> Ringbuffer<T>::Ringbuffer()
{
    buffer = nullptr;
    length = 1;
    head = 0;
    tail = 0;
}


/**
 * @brief Construct a new Ringbuffer< T>:: Ringbuffer object
 * 
 * @tparam T 
 * @param length size of the required ring buffer
 */
template <typename T> Ringbuffer<T>::Ringbuffer(T* buffer_ptr, uint16_t buff_length)
{

    // initialize the buffer
    buffer = buffer_ptr;
    length = buff_length;

    head = 0;

    // point the tail to the last position in the buffer
    tail = length - 1;


}

/**
 * @brief Destroy the Ringbuffer< T>:: Ringbuffer object
 * 
 * @tparam T 
 */
template <typename T> Ringbuffer<T>::~Ringbuffer()
{
    ;
}


/**
 * @brief checks if the buffer is empty. 
 * @remark empty means storage_available = length -1 
 * 
 * @tparam T 
 * @return true on empty
 * @return false on not empty
 */
template <typename T> bool Ringbuffer<T>::is_empty() const
{

    return ((tail + 1) % length) == head;

}

/**
 * @brief get the free space in the buffer
 * 
 * @tparam T data type of buffer
 * @return uint16_t available space in elements
 */
template <typename T> uint16_t Ringbuffer<T>::storage_available() const
{
    uint16_t ii;
    ii = (tail < head) ? (tail + (sizeof(T) * length) - head) : (tail - head);

    return ii;
}

/**
 * @brief checks if the buffer is full (i.e. if head pointer == tail pointer)
 * 
 * @tparam T 
 * @return true 
 * @return false 
 */
template <typename T> bool Ringbuffer<T>::is_full() const
{
    return head == tail;
}


/**
 * @brief puts data object into the buffer
 * 
 * @tparam T object type
 * @param data object
 * @return uint16_t return code
 * @retval 0 success
 * @retval 1 failed (buffer full)
 */
template <typename T> uint16_t Ringbuffer<T>::put(T data)
{

    if(!is_full())    
    {
        buffer[head] = data;
        head = (head + 1) % length;
    }
    else
    {
        return 1;
    }
    return 0;

}

/**
 * @brief injects data into the ringbuffer
 * 
 * @tparam T datatype
 * @param data data pointer
 * @param size length of the array or to inject data
 * @return uint16_t number of successfully injected objects
 */
template <typename T> uint16_t Ringbuffer<T>::put(T *data, uint16_t size)
{
    // loops over all data and inserts it
    for(uint16_t ii = 0; ii < size; ii++)
    {
        if(put(data[ii]) != 0)
        {
            // on error: return the nubmer of successful injected data
            return ii;
        }
    }

}


/**
 * @brief fetches an object from the ringbuffer
 * 
 * @tparam T object type
 * @return T object from the buffer or nullptr on empty
 */
template <typename T> T Ringbuffer<T>::pop()
{
    T obj;
    if(!is_empty())
    {
        // advance tail
        tail = (tail + 1) % length;
        obj = buffer[tail];
    }

    return obj;
}


template <typename T> uint16_t Ringbuffer<T>::in_storage() const
{
    return length - storage_available() -1;
}

/**
 * @brief helper function to empty all data from buffer
 * 
 * @tparam T 
 */
template <typename T> void Ringbuffer<T>::clear_buffer() 
{
    head = (tail + 1) % length;
}

#endif
