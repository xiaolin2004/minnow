#include "byte_stream.hh"

using namespace std;

ByteStream::ByteStream( uint64_t capacity ) : capacity_( capacity ), buff( std::deque<char>() )
{
  closed = false;
  reading = false;
}

bool Writer::is_closed() const
{
  return closed;
}

void Writer::push( string data )
{
  auto data_ = data.data();
  if ( *data_ == '\0' && data.length() == 0 )
    return;
  if ( !this->is_closed() ) {
    for ( size_t i = 0; i < data.length(); i++ ) {
      if ( available_capacity() != 0 ) {
        set_buffer().push_back( data_[i] );
        byte_pushed++;
      } else
        return;
    }
    return;
  } else {
    set_error();
    return;
  }
}

void Writer::close()
{
  closed = true;
}

uint64_t Writer::available_capacity() const
{
  return capacity_ - ( byte_pushed - byte_popped );
}

uint64_t Writer::bytes_pushed() const
{
  return byte_pushed;
}

bool Reader::is_finished() const
{
  return closed && ( bytes_buffered() == 0 );
}

uint64_t Reader::bytes_popped() const
{
  return byte_popped;
}

string_view Reader::peek() const
{
  reading = true;
  string_view data_str( &get_buffer()[0], 1 );
  reading = false;
  return data_str;
}

void Reader::pop( uint64_t len )
{
  for ( auto i = len; i > 0; i-- ) {
    set_buffer().pop_front();
    byte_popped++;
  }
}

uint64_t Reader::bytes_buffered() const
{
  return byte_pushed - byte_popped;
}
