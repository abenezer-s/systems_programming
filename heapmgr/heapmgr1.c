//heap manager//

#include "heapmgr.h"
#include "chunk.h"
#include <stddef.h>
#include <stdio.h>  /* For debugging */
#include <assert.h>

#define __USE_MISC
#include <unistd.h>
// heap state
static Chunk_T oHeapStart = NULL;
static Chunk_T oHeapEnd = NULL;
static Chunk_T oFreeList = NULL;

enum {MIN_UNITS_FROM_OS = 1024};
#ifndef NDEBUG

static int HeapMgr_isValid(void)

/* Return 1 (TRUE) iff the heap manager is in a valid state. */

{
   Chunk_T oChunk;
   Chunk_T oPrevChunk;

   if (oHeapStart == NULL)
      {fprintf(stderr, "Uninitialized heap start\n"); return 0; }
   if (oHeapEnd == NULL)
      {fprintf(stderr, "Uninitialized heap end\n"); return 0; }

   if (oHeapStart == oHeapEnd)
   {
      if (oFreeList == NULL)
         return 1;
      else
         {fprintf(stderr, "Inconsistent empty heap\n"); return 0; }
   }

   /* Traverse memory. */
   for (oChunk = oHeapStart;
        oChunk != NULL;
        oChunk = Chunk_getNextInMem(oChunk, oHeapStart, oHeapEnd))
      if (! Chunk_isValid(oChunk, oHeapStart, oHeapEnd))
         return 0;

   /* Traverse the free list. */
   oPrevChunk = NULL;
   for (oChunk = oFreeList;
        oChunk != NULL;
        oChunk = Chunk_getNextInList(oChunk))
   {
      if (! Chunk_isValid(oChunk, oHeapStart, oHeapEnd))
         return 0;
      if (oPrevChunk != NULL)
         if (Chunk_getNextInMem(oPrevChunk, oHeapStart, oHeapEnd)
             == oChunk)
            {fprintf(stderr, "Uncoalesced chunks\n"); return 0; }
      oPrevChunk = oChunk;
   }

   return 1;
}

#endif


Chunk_T HeapMgr_getMoreMemory(Chunk_T oPrevChunk, size_t uiUnits)
    //get more memory if there if we have reached heapend. by increasing brk.

{
    Chunk_T oChunk;
    Chunk_T oHeader;


    if (uiUnits < MIN_UNITS_FROM_OS)
      uiUnits = MIN_UNITS_FROM_OS;

   /* Move the program break. */
    oChunk = (Chunk_T)sbrk(uiUnits * Chunk_getUnitSize());
    oHeader = oChunk;
    if (oChunk == (Chunk_T)-1)
      return NULL;

   /* Determine where the program break is now, and note the
      end of the heap. */
    oHeapEnd = (Chunk_T)sbrk(0);
   /* Set the fields of the new chunk's oHeader and footer. */
    Chunk_setUnits(oChunk, uiUnits);
    Chunk_setNextInList(oChunk, NULL);
    /* Add the new chunk to the begining of the free list. */
    if(oPrevChunk != NULL)
    {
    Chunk_setNextInList(oChunk, oPrevChunk);
    Chunk_setPrevInList(oPrevChunk, oChunk);
    oFreeList = oChunk;

    } else
      {
    Chunk_setPrevInList(oChunk, NULL);
    oFreeList = oChunk;

      }

    Chunk_setStatus(oChunk, CHUNK_FREE);


   /* Coalesce the new chunk and the previous one if appropriate. */
  // if (oPrevChunk != NULL)
  //    if (Chunk_getNextInMem(oPrevChunk, oHeapStart, oHeapEnd)
  //        == oChunk)
  //    {
  //       Chunk_setUnits(oPrevChunk,
  //          Chunk_getUnits(oPrevChunk) + uiUnits);
  //       Chunk_setNextInList(oPrevChunk, NULL);
  //       oChunk = oPrevChunk;
  //    }
    //size_t footersUnits = Chunk_getUnits(oFooter);
    //printf("footersUnits: %zu\n", footersUnits);
   return oChunk;
}



static Chunk_T HeapMgr_useChunk(Chunk_T oChunk,
   Chunk_T oPrevChunk, size_t uiUnits)
{
    Chunk_T oNewChunk;
   size_t uiChunkUnits = Chunk_getUnits(oChunk);

   /* If oChunk is close to the right size, then use it. */
   if (uiChunkUnits < uiUnits + (size_t)MIN_UNITS_PER_CHUNK)
   {
      if (Chunk_getPrevInList(oChunk) == NULL)
      /*either freelist = bestfit and there is no next*/
      {
         oFreeList = Chunk_getNextInList(oChunk);
         if(Chunk_getNextInList(oChunk)!= NULL)
         {
            Chunk_setPrevInList(Chunk_getNextInList(oChunk), NULL);
         }
      } else
         {
           Chunk_T prevchunk = Chunk_getPrevInList(oChunk);
           Chunk_T nextchunk = Chunk_getNextInList(oChunk);

           if(prevchunk != NULL)
           {
               Chunk_setNextInList(prevchunk, nextchunk);
           }
           if(nextchunk != NULL)
           {
               Chunk_setPrevInList(nextchunk, prevchunk);
           }
         }
         Chunk_setStatus(oChunk,CHUNK_INUSE);
         return oChunk;
   }  
   /* oChunk is too big, so use the tail end of it. keep ochunks previous chunck.  */
   
   Chunk_T prevInList = Chunk_getPrevInList(oChunk);
   
   Chunk_setUnits(oChunk, uiChunkUnits - uiUnits);

   oNewChunk = Chunk_getNextInMem(oChunk, oHeapStart, oHeapEnd);
   Chunk_setUnits(oNewChunk, uiUnits);
   Chunk_setNextInList(oNewChunk,NULL);
   Chunk_setPrevInList(oNewChunk, NULL);
   Chunk_setStatus(oNewChunk, CHUNK_INUSE);
//check new chunks fields
    size_t newuni = Chunk_getUnits(oNewChunk);
    Chunk_T newprevInList = Chunk_getPrevInList(oNewChunk);
   Chunk_T newnextInList = Chunk_getNextInList(oNewChunk);
   // size_t newfooteruni = Chunk_getFooterUnits(oNewChunk);
    int newchunkStat =Chunk_getStatus(oNewChunk);


   Chunk_setPrevInList(oChunk, prevInList);
   // check chk1s feilds
   size_t uni = Chunk_getUnits(oChunk);
   //size_t footeruni = Chunk_getFooterUnits(oChunk);
    prevInList = Chunk_getPrevInList(oChunk);
   Chunk_T nextInList = Chunk_getNextInList(oChunk);

   int chunkStat = Chunk_getStatus(oChunk);


   return oNewChunk;
}

Chunk_T HeapMgr_coalsce(Chunk_T freelist, size_t uiUnits)
// coalece when possible while maintaining the order of the frelist
{
   size_t curChunkSize = 0;
   Chunk_T curChunk;
   Chunk_T foundfit = NULL;

   for (curChunk = freelist; 
         curChunk != NULL; 
         curChunk = Chunk_getNextInList(curChunk))
   {  
      Chunk_T nextInMem = Chunk_getNextInMem(curChunk, oHeapStart, oHeapEnd);
      Chunk_T prevInMem = Chunk_getPrevInMem(curChunk, oHeapStart, oHeapEnd);
      Chunk_T prevInList = Chunk_getPrevInList(curChunk);
      Chunk_T nextInList = Chunk_getNextInList(curChunk);
     
      
      if(nextInMem != NULL)
      { /*coalesce with next*/
         if(Chunk_getStatus(nextInMem) == CHUNK_FREE)
         {

            Chunk_setUnits(curChunk, Chunk_getUnits(curChunk) + Chunk_getUnits(nextInMem));
            

            if(nextInList == nextInMem)
            {
               Chunk_setNextInList(curChunk, Chunk_getNextInList(nextInMem));
               if(Chunk_getNextInList(nextInMem) != NULL)
               {
                  Chunk_setPrevInList(Chunk_getNextInList(nextInMem), curChunk);
               }
            }

            if(Chunk_getPrevInList(nextInMem) != NULL && Chunk_getPrevInList(nextInMem) != curChunk)
            {
               if(Chunk_getNextInList(nextInMem)!= NULL)
               {
                  Chunk_setPrevInList(Chunk_getNextInList(nextInMem), nextInList);
               }
               Chunk_setNextInList(Chunk_getPrevInList(nextInMem),  Chunk_getNextInList(nextInMem));
               Chunk_setPrevInList(nextInList, curChunk);
            } 
            if(prevInList == nextInMem)
            {
               Chunk_setPrevInList(curChunk, Chunk_getPrevInList(prevInMem));
            } else
               {
                  Chunk_setPrevInList(curChunk, prevInList);
               }

            nextInMem = curChunk;
            if(Chunk_getUnits(curChunk) >= uiUnits)
            {
               foundfit = curChunk;
            }

         }
      }

      if(prevInMem != NULL)
      {/*coalesce with prev*/
         if(Chunk_getStatus(prevInMem) == CHUNK_FREE)
         {
            prevInList = Chunk_getPrevInList(curChunk);
            nextInList = Chunk_getNextInList(curChunk);
            Chunk_T prevInMemsPrevInList = Chunk_getPrevInList(prevInMem);

            Chunk_setUnits(prevInMem, Chunk_getUnits(curChunk)+ Chunk_getUnits(prevInMem));
            
            if(nextInList != prevInMem)
            {
               
               if(Chunk_getNextInList(prevInMem) != NULL && Chunk_getNextInList(prevInMem) != curChunk )
               {
               Chunk_setPrevInList(Chunk_getNextInList(prevInMem), nextInList);
               //Chunk_setNextInList(nextInList, Chunk_getNextInList(prevInMem));
               }

               if(prevInList == prevInMem)
               {
                  Chunk_setPrevInList(prevInMem, prevInMemsPrevInList);
               }

               Chunk_setNextInList(prevInMem, nextInList);
               if (nextInList != NULL)
               {
               Chunk_setPrevInList(nextInList, prevInMem);
               }

               //Chunk_setNextInList(nextInList, Chunk_getNextInList(prevInMem));
            }

            if(nextInList == prevInMem)
            {
               Chunk_setNextInList(prevInMem, Chunk_getNextInList(prevInMem));
               if(Chunk_getNextInList(prevInMem) != NULL)
               {
                  Chunk_setPrevInList(Chunk_getNextInList(prevInMem), prevInMem);
               }

               if(prevInList != NULL)
               {
                  Chunk_setNextInList(prevInList, prevInMem);
               }
            }

            if(prevInList != prevInMem)
            {
               Chunk_setPrevInList(prevInMem,  prevInList);
               if(prevInList == NULL)
               {
                  oFreeList = prevInMem;
               }
            }

            curChunk = prevInMem;
            if(Chunk_getUnits(curChunk) >= uiUnits)
            {
               foundfit = curChunk;
            }

            if(Chunk_getNextInList(curChunk) == NULL && Chunk_getPrevInList(curChunk) != NULL)
            {
               curChunk = prevInList;
            }
         }
      }
   }
   return foundfit;
}

void *HeapMgr_malloc(size_t uiBytes)
    // allocate nbytes and return a poitnter to the begining of the payload.

{
   Chunk_T oChunk;
   Chunk_T oPrevChunk;
   Chunk_T oPrevPrevChunk;
   size_t uiUnits;
   size_t uiUnitSize;

   if (uiBytes == 0)
      return NULL;

   /* Initialize the heap manager if this is the first call. */
   if (oHeapStart == NULL)
   {
      oHeapStart = (Chunk_T)sbrk(0);
      oHeapEnd = oHeapStart;
   }

   assert(HeapMgr_isValid());

   /* Determine the number of units the new chunk should contain. */
   uiUnitSize = Chunk_getUnitSize();
   while (uiBytes % uiUnitSize != 0)
      uiBytes++;
   uiUnits = uiBytes / uiUnitSize;
   uiUnits += 2;  /* Allow room for a header and footer. */

   oPrevPrevChunk = NULL;
   oPrevChunk = NULL;
   Chunk_T bestFit = NULL;
// allocate  BEST FIT block
   for (oChunk = oFreeList;
        oChunk != NULL;
        oChunk = Chunk_getNextInList(oChunk))
   {
      if(Chunk_getUnits(oChunk) >= uiUnits )
      {
        Chunk_T prevInList = NULL;
        if( oChunk != oFreeList)
        {
            prevInList = Chunk_getPrevInList(oChunk);
        }
        oChunk = HeapMgr_useChunk(oChunk, prevInList, uiUnits);
        assert(HeapMgr_isValid());
        return (void*)((char*)oChunk + uiUnitSize);
      }
      oPrevPrevChunk = oPrevChunk;
      oPrevChunk = oChunk;
   }  


//  COALECE if freelist not null 
Chunk_T coalesced = NULL;
   if(oFreeList != NULL)
   {
      coalesced = HeapMgr_coalsce(oFreeList, uiUnits);
   }

  
   if(coalesced != NULL)
      {
         oChunk = HeapMgr_useChunk(coalesced, oPrevChunk, uiUnits);
         size_t unitsEnd = Chunk_getUnits(oChunk);
         size_t heapbegin = (size_t)oHeapStart;
         return (void*)((char*)oChunk + uiUnitSize);

      }
/* Ask the OS for more memory, and create a new chunk (or expand
      the existing chunk) at the end of the free list. */

   oChunk =  HeapMgr_getMoreMemory(oPrevChunk, uiUnits);
   if (oChunk == NULL)
   {
      //assert(HeapMgr_isValid());
      return NULL;
   }

   /* If the new large chunk was coalesced with the previous chunk,
      then reset the previous chunk. */
   if (oChunk == oPrevChunk)
      oPrevChunk = oPrevPrevChunk;

   /* oChunk is big enough, so use it. */
   Chunk_T prevInList = NULL;
   if(oChunk != oFreeList)
   {
     prevInList = Chunk_getPrevInList(oChunk);
   }
   oChunk = HeapMgr_useChunk(oChunk, prevInList, uiUnits);
   //assert(HeapMgr_isValid());
   return (void*)((char*)oChunk + uiUnitSize);
}

void HeapMgr_free(void *pvBytes)
{
    Chunk_T oChunk = (Chunk_T) (pvBytes - Chunk_getUnitSize());
    Chunk_T prevFreeBegining = oFreeList;
    Chunk_setPrevInList(prevFreeBegining, oChunk);
    Chunk_setNextInList(oChunk, prevFreeBegining);
    Chunk_setPrevInList(oChunk, NULL);
    Chunk_setStatus(oChunk, CHUNK_FREE);
    oFreeList = oChunk;
    Chunk_T prevfree = Chunk_getPrevInList(oChunk);
    Chunk_T nextfree = Chunk_getNextInList(oChunk);
    Chunk_T prevfree1 = Chunk_getPrevInList(nextfree);
    Chunk_T nxnxfree1 =Chunk_getNextInList(nextfree);

    
}




