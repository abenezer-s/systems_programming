//heap manager//

#include "heapmgr.h"
#include "chunk.h"
#include <stddef.h>
#include <stdio.h>  /* For debugging */
#include <assert.h>

#define __USE_MISC
#include <unistd.h>

#define NUMBEROFBINS 128
#define MAXUNITS 228891

// heap state
static Chunk_T oHeapStart = NULL;
static Chunk_T oHeapEnd = NULL;
static Chunk_T oFreeList = NULL;

Chunk_T bins[129] = {NULL};
enum {MIN_BYTES_FROM_OS = 3662256};
size_t totalHeapMem; // total bytes ACCCUIRED from heap
double scale = 0.115;
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
int findBin(size_t uiUnits)
{
      // find appropriate bin for free request
   int i;
   int binNum;
   int baseBin,NBU,BBU,CBU; // NBU: Next Bin Unit, BBU: Base Bin Unit, Cur Bin Units;
   
   baseBin = 50;
   BBU = 52;
   CBU = BBU;
   NBU = CBU + (int)(CBU * scale);
   for(i = 1;
      i <= NUMBEROFBINS;
      i++)
   {
      if(uiUnits > MAXUNITS)
      {
         binNum = NUMBEROFBINS;
         break;
      }
   // is  uiUnits greater than 52 if so scale
      if(uiUnits > BBU)
      {
         binNum = 51;
         if(uiUnits > NBU)
         {
            CBU = NBU;
            NBU = CBU + (int)(CBU * scale);

            binNum++;
            continue;
         } else  
            {
               break;
            }
      }
   // uiUnits not greater than 52
      if(uiUnits > (i + 2))
      {
         continue;
      } else
         {
            binNum = i;
            break;
         }
   }
   return binNum;
}


int chopMemory(Chunk_T oChunk, int obinNum, size_t numOfBins, size_t uiUnits)
{
   //chop memmory to approapriate sizes according to the binsizes and bin number. 
   // return 1 if succesful 0 if otherwise.
      Chunk_T curChunk;
      size_t curBinUnits, unitSize, binUnitInc, unUsedHeap ; //number of units for a given bin
      size_t unitofcur;
      int binNum;
      size_t chopped = 0;
      size_t binSet = 0;
      
      unitSize = Chunk_getUnitSize();
      if(obinNum == 0) // initializing the bin data structure.
      {
         binNum = 1;
         curBinUnits =  (binNum * unitSize + (2 * unitSize) ) / unitSize;
      }else // chopping a memory when we are refilling a bin.
         {
            binNum = obinNum;
            curBinUnits = uiUnits;
         }
      // number of chunks in a given bin.
      size_t binChunkLimit;
      
      if(curBinUnits > MAXUNITS)
      {
         binChunkLimit = 1;
      } else
         {
            binChunkLimit = MIN_BYTES_FROM_OS / (curBinUnits * unitSize); 
         }
      size_t curbinbytes = binChunkLimit * curBinUnits * unitSize;
      size_t totalHeapUsed = curbinbytes;
      for(curChunk = oChunk;
         binSet < numOfBins;
         curChunk = Chunk_getNextInMem(curChunk, oHeapStart, oHeapEnd)) 
      {  
         Chunk_setUnits(curChunk, curBinUnits);
          unitofcur = Chunk_getUnits(curChunk);
         Chunk_setNextInList(curChunk, bins[binNum]);
         Chunk_setPrevInList(curChunk, NULL);
         Chunk_T nextInList = Chunk_getNextInList(curChunk);
         if(nextInList != NULL)
         {
            Chunk_setPrevInList(nextInList, curChunk);
         }
         bins[binNum] = curChunk; 
         chopped += 1;
         if(chopped == binChunkLimit && numOfBins == 1)
         {
            return 1;
            
         } else if(chopped == binChunkLimit) 
            {
               //set values for new bin
               chopped = 0; 
               binNum += 1;
               // should we start scale?
               if(binNum > 50)
               {
                  binUnitInc = (int)(curBinUnits * scale);
                  curBinUnits += binUnitInc;

               }else
                  {
                     curBinUnits = (binNum * unitSize + (2 * unitSize) ) / unitSize;
                  }

               binChunkLimit = MIN_BYTES_FROM_OS / (curBinUnits * unitSize);
               curbinbytes = binChunkLimit * curBinUnits * unitSize;
               totalHeapUsed += curbinbytes;
               binSet += 1;
               //set the program break back to the end of the data structure.
               if(binSet == NUMBEROFBINS && totalHeapUsed != totalHeapMem)
               {
                  unUsedHeap = totalHeapMem - totalHeapUsed;
                  Chunk_T oldEnd = sbrk(-unUsedHeap);
                  oHeapEnd = sbrk(0);
               }
            }
      }
   
   
   return 1;
}


Chunk_T HeapMgr_getMoreMemory(int obinNum, size_t uiUnits)
//get more memomry from os and initialize the datastructure if binNum is NULL
{  
   // initiallize the data structre if binNum equals null
   Chunk_T oChunk;
   int binNum, chopper;
   size_t uiUnitSize;
   if((binNum = obinNum) == 0 && uiUnits == 0)
   {  
      uiUnitSize = MIN_BYTES_FROM_OS;      
   /* Move the program break. */
      oChunk = (Chunk_T)sbrk((NUMBEROFBINS * uiUnitSize));
      if (oChunk == (Chunk_T)-1)
      {
         return NULL;
      }
      /* Determine where the program break is now, and note the
      end of the heap. */
      totalHeapMem = NUMBEROFBINS * uiUnitSize;
      oHeapEnd = (Chunk_T)sbrk(0);
      
      chopper = chopMemory(oChunk, binNum, NUMBEROFBINS, uiUnits);
      if(chopper != 0)
         {
            return (Chunk_T)1;
         }
      return NULL;
   } else
      {
         /* Move the program break. */
         uiUnitSize = uiUnits * Chunk_getUnitSize();
         if ( uiUnitSize < MIN_BYTES_FROM_OS)
         {  
            uiUnitSize = MIN_BYTES_FROM_OS;
         } 

         oChunk = (Chunk_T)sbrk(uiUnitSize);
         if (oChunk == (Chunk_T)-1)
         {
            return NULL;
         }
         /* Determine where the program break is now, and note the
         end of the heap. */
         oHeapEnd = (Chunk_T)sbrk(0);
         // chop memory starting from oChunk 
         chopper = chopMemory(oChunk, binNum, 1, uiUnits);
         if(chopper != 0)
         {
            return (Chunk_T)1;
         }
         return NULL;
      }

}

void *HeapMgr_malloc(size_t uiBytes)
    // allocate uibytes and return a poitnter to the begining of the payload.

{
   Chunk_T oChunk;
   
   size_t uiUnits;
   size_t uiUnitSize;

   if (uiBytes == 0)
      return NULL;

   /* Initialize the heap manager if this is the first call. */
   if (oHeapStart == NULL)
   {
      oHeapStart = (Chunk_T)sbrk(0);
      oHeapEnd = oHeapStart;
      HeapMgr_getMoreMemory(0, 0);
   }

   assert(HeapMgr_isValid());

   /* Determine the number of units the new chunk should contain. */
   uiUnitSize = Chunk_getUnitSize();
   while (uiBytes % uiUnitSize != 0)
      uiBytes++;
   uiUnits = uiBytes / uiUnitSize;
   uiUnits += 2;  /* Allow room for a header and footer. */
// find appropriate bin for uiUnits
int binNum = findBin(uiUnits);
//allocate the first chunk at bin[binNum]. Maintain the list. if bin[binNum] is NULL then refill bin[binNum]
   Chunk_T moreMemory;
   if(uiUnits > MAXUNITS)
   {
      
      moreMemory = HeapMgr_getMoreMemory(binNum, uiUnits);
      if(moreMemory != NULL)
      {
         oChunk = bins[binNum];
         if(oChunk != NULL)
         bins[binNum] = Chunk_getNextInList(oChunk);
         Chunk_setStatus(oChunk, CHUNK_INUSE);
         Chunk_setNextInList(oChunk, NULL);

        assert(HeapMgr_isValid());
         return (void*)((char*)oChunk + uiUnitSize);
      }
   }
size_t binbinNumNULL = 0;
//allocate 
   if(bins[binNum] != NULL)
   {
      oChunk = bins[binNum];

      bins[binNum] = Chunk_getNextInList(oChunk);
      Chunk_setStatus(oChunk, CHUNK_INUSE);
      Chunk_setNextInList(oChunk, NULL);

      assert(HeapMgr_isValid());
      return (void*)((char*)oChunk + uiUnitSize);
   }
binbinNumNULL++;
//refill bins[binNum]
   moreMemory = HeapMgr_getMoreMemory(binNum, uiUnits);
   if (moreMemory != NULL)
   {
      oChunk = bins[binNum];

      bins[binNum] = Chunk_getNextInList(oChunk);
      Chunk_setStatus(oChunk, CHUNK_INUSE);
      Chunk_setNextInList(oChunk, NULL);

      assert(HeapMgr_isValid());
      return (void*)((char*)oChunk + uiUnitSize);
   }

}



void HeapMgr_free(void *pvBytes)
{
   Chunk_T nextInList;
   Chunk_T oChunk = (Chunk_T) (pvBytes - Chunk_getUnitSize());
   size_t uiUnits = Chunk_getUnits(oChunk);
         
   int bin = findBin(uiUnits);

   // insert oChunk at bins
   nextInList = bins[bin];
   Chunk_setNextInList(oChunk, nextInList);
   if(nextInList != NULL)
   {
      Chunk_setPrevInList(nextInList, oChunk);
   }
    
}

