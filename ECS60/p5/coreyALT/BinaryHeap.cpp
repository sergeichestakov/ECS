        #include "BinaryHeap.h"
        #include "vertex.h"
        
        /**
         * Construct the binary heap.
         * capacity is the capacity of the binary heap.
         */
        template <class Comparable>
        BinaryHeap<Comparable>::BinaryHeap( int capacity, int col )
          : currentSize(0)
        {
            maxSize = capacity;
            array = new Comparable[maxSize];
            tableCol = col;
        }

        /**
         * Insert item x into the priority queue, maintaining heap order.
         * Duplicates are allowed.
         * Throw Overflow if container is full.
         */
        template <class Comparable>
        void BinaryHeap<Comparable>::insert( const Comparable & x )
        {
            /*if( isFull( ) )
                throw Overflow( );*/

            // Initial hole
            int hole = ++currentSize;
            
            //Corey's magic size reducing sauce
            if((hole & 1) == 0 && table[ ((Vertex &) array[hole>>1]).city ][tableCol] == 1) {
                hole = hole>>1;
                currentSize--;
            }
            
            //Percolate up
            for( ; hole > 1 && x < array[ hole>>1 ]; hole >>= 1 )
                array[ hole ] = array[ hole>>1 ];
            
            
            array[ hole ] = x;
        }

        /**
         * Find the smallest item in the priority queue.
         * Return the smallest item, or throw Underflow if empty.
         */
        template <class Comparable>
        const Comparable & BinaryHeap<Comparable>::findMin( ) const
        {
            //if( isEmpty( ) )
            //    throw Underflow( );
            return array[ 1 ];
        }

        /**
         * Remove the smallest item from the priority queue.
         * Throw Underflow if empty.
         */
        template <class Comparable>
        void BinaryHeap<Comparable>::deleteMin( )
        {
            //if( isEmpty( ) )
            //    throw Underflow( );

            //array[ 1 ] = array[ currentSize-- ];
            percolateEmptyDown( 1 );
        }

        /**
         * Remove the smallest item from the priority queue
         * and place it in minItem. Throw Underflow if empty.
         */
        template <class Comparable>
        void BinaryHeap<Comparable>::deleteMin( Comparable & minItem )
        {
            //if( isEmpty( ) )
            //    throw Underflow( );

            minItem = array[ 1 ];
            //array[ 1 ] = array[ currentSize-- ];
            percolateEmptyDown( 1 ); //move the empty space down
        }

        /**
         * Establish heap order property from an arbitrary
         * arrangement of items. Runs in linear time.
         */
        template <class Comparable>
        void BinaryHeap<Comparable>::buildHeap( )
        {
            for( int i = currentSize>>1; i > 0; i-- )
                percolateDown( i );
        }

        /**
         * Test if the priority queue is logically empty.
         * Return true if empty, false otherwise.
         */
        template <class Comparable>
        bool BinaryHeap<Comparable>::isEmpty( ) const
        {
            return currentSize == 0;
        }

        /**
         * Test if the priority queue is logically full.
         * Return true if full, false otherwise.
         */
        template <class Comparable>
        bool BinaryHeap<Comparable>::isFull( ) const
        {
            return currentSize == maxSize;
        }

        /**
         * Make the priority queue logically empty.
         */
        template <class Comparable>
        void BinaryHeap<Comparable>::makeEmpty( )
        {
            currentSize = 0;
        }

        /**
         * Internal method to percolate down in the heap.
         * hole is the index at which the percolate begins.
         */
        template <class Comparable>
        void BinaryHeap<Comparable>::percolateDown( int hole )
        {
/* 1*/      int child;
/* 2*/      Comparable tmp = array[ hole ];

/* 3*/      for( ; (hole<<1) <= currentSize; hole = child )
            {
/* 4*/          child = hole<<1;
/* 5*/          if( child != currentSize && array[ child + 1 ] < array[ child ] )
/* 6*/              child++;
/* 7*/          if( array[ child ] < tmp )
/* 8*/              array[ hole ] = array[ child ];
                else
/* 9*/              break;
            }
/*10*/      array[ hole ] = tmp;
        }
        
        template <class Comparable>
        void BinaryHeap<Comparable>::percolateEmptyDown( int hole )
        {
            int child;
            
            for( ; (hole<<1) <= currentSize; hole = child ) {
                child = hole<<1;
                
                if(child != currentSize && array[child+1] < array[child]) {
                    child++;
                }
                array[hole] = array[child];
            }
            
            array[hole] = array[currentSize];
            currentSize--;
            percolateUp(hole);
        }
        
        template <class Comparable>
        void BinaryHeap<Comparable>::percolateUp(int hole) {
            Comparable x = array[hole];
            
            //Percolate up
            for( ; hole > 1 && x < array[ hole>>1 ]; hole >>= 1 )
                array[ hole ] = array[ hole>>1 ];
            
            
            array[ hole ] = x;
        }