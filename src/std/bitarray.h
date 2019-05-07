#pragma once

#define block_size (sizeof( long long ) * 8)

class( BitArray, {
	long long int*  ptr;
	size_t 		len;

});

BitArray bitarray(const size_t len)
{
	BitArray this = {NULL, len};
 	
	const size_t l          = max( 1 , ceilf( (float)len / block_size  ) );
	this.ptr                = alloc_zero(l * block_size);
	 this.len                = len;
	return this;
}

void bitarray_set( BitArray* this, const size_t bit )
{   
	this->ptr[ bit / block_size ] |= 1ull << (bit % block_size);
}

void bitarray_clear( BitArray* this, const size_t bit )
{    
	this->ptr[ bit / block_size ] &= ~(1ull << (bit % block_size));
}

bool bitarray_check( BitArray* this, const size_t bit )
{
	return (this->ptr[ bit / block_size ] >> (bit % block_size)) & 1ull;
}

bool bitarray_check_mask(BitArray* this, const BitArray other)
{
	(void)(this);
	(void)(other);
	return true;
}