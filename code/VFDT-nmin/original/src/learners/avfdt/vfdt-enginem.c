#include "vfdt-engine.h"
#include "memory.h"
#include <math.h>

#include <stdlib.h> /* for system for bootstrapc45 */

#define lg(x)       (log(x) / log(2))


// Global variables for stuf F
#pragma GCC push_options
#pragma GCC optimize ("O0")

static int processExCounterF           = 4000000;
static int filterInstanceCounter       = 7000000;
static int addExampleGrowingCounter    = 8000000;
static int addExampleGrowingCounterF   = 9000000;
static int reactivateScancounter       = 11000000;
static int flushCounter                = 12000000;
static int leafifyCounter              = 13000000;
static int updateStatsCounter          = 14000000;
static int checkSplitCounter           = 15000000;
static int checkSplitCounterF          = 16000000;
static int bestAttrCounter             = 17000000;
static int prepruneCounter             = 18000000;
static int makeLeafCounter             = 19000000;
static int discreteCounter             = 21000000;
static int contCounter                 = 22000000;
static int discreteCounterF            = 23000000;
static int contCounterF                = 24000000;
static int insidePrePruneCounter       = 25000000;
static int disableLeafCounter          = 26000000;

#pragma GCC pop_options

static void _DecisionTreeWrite(DecisionTreePtr dt);
static float _BonferonniDelta(VFDTPtr vfdt, int numChoices);

//SimRoiStart();
static void _GrowingDataFlushExampleCache(VFDTGrowingDataPtr gd) {
   int i;
   if(gd->exampleCache != 0) {
      for(i = VALLength(gd->exampleCache) - 1 ; i >= 0 ; i--) {
         ExampleFree(VALRemove(gd->exampleCache, i));
      }
   }
}

static void _GrowingDataFreeExampleCache(VFDTGrowingDataPtr gd) {
   if(gd->exampleCache != 0) {
      _GrowingDataFlushExampleCache(gd);
      VALFree(gd->exampleCache);
      gd->exampleCache = 0;
   }
}
// EEVFDT: 3rd May. Adding the variable eChunk. 

VFDTPtr VFDTNew(ExampleSpecPtr spec, float splitConfidence,
           float tieConfidence, int gChunk) {
   VFDTGrowingDataPtr gd = MNewPtr(sizeof(VFDTGrowingData));
   VFDTPtr vfdt = MNewPtr(sizeof(VFDT));
   AttributeTrackerPtr at;

   vfdt->spec = spec;
   vfdt->dtree = DecisionTreeNew(spec);

   at = AttributeTrackerInitial(vfdt->spec);
   gd->egs = ExampleGroupStatsNew(vfdt->spec, at);
                    

   vfdt->cacheExamples = 1;
   vfdt->recoverMinimum = 3 * 1024 * 1024;
   if(vfdt->cacheExamples) {
      gd->exampleCache = VALNew();
   } else {
      gd->exampleCache = 0;
   }

   //   gd->startedOnExampleNum = 0;
   gd->seenExamplesCount = 0;
   gd->seenSinceLastProcess = 0;

   gd->treeLevel = 0;
   gd->prePruned = 0;

   // EEFVDT: Included: 3rd May 2017
   gd->eChunk = gChunk;

   DecisionTreeSetTypeGrowing(vfdt->dtree);
   DecisionTreeSetGrowingData(vfdt->dtree, gd);

   vfdt->processChunkSize = 200;
   vfdt->splitConfidence = splitConfidence;
   vfdt->tieConfidence = tieConfidence;

   vfdt->useGini = 0;
   vfdt->messageLevel = 0;
   vfdt->examplesSeen = 0;
   vfdt->numBoundsUsed = 0;
   vfdt->numGrowing = 1;
   vfdt->maxAllocationBytes = 10000;
   vfdt->maxAllocationBytes *= 1024;
   vfdt->maxAllocationBytes *= 1024;

   vfdt->reactivateLeaves = 1;
   vfdt->highestDeactivatedIndex = 1.0;
   vfdt->reactivateScanPeriod = 1473777;

   vfdt->batchMode = 0;
   vfdt->prePruneTau = 0.0;
   vfdt->laplace = 0;

   /* needs the completed vfdt, so make sure it happens after that */
   gd->splitConfidence = _BonferonniDelta(vfdt, AttributeTrackerNumActive(at));

   return vfdt;
}

void VFDTFree(VFDTPtr vfdt) {
   int i;
   VoidAListPtr list = VALNew();
   VFDTGrowingDataPtr gd;

   DecisionTreeGatherGrowingNodes(vfdt->dtree, list);

   for(i = VALLength(list) - 1 ; i >= 0 ; i--) {
      gd = (VFDTGrowingDataPtr)DecisionTreeGetGrowingData(VALIndex(list,i));

      _GrowingDataFreeExampleCache(gd);

      ExampleGroupStatsFree(gd->egs);
      MFreePtr(gd);
   }

   VALFree(list);

   DecisionTreeFree(vfdt->dtree);
   MFreePtr(vfdt);
}

void VFDTSetMessageLevel(VFDTPtr vfdt, int value) {
   vfdt->messageLevel = value;
}

void VFDTSetMaxAllocationMegs(VFDTPtr vfdt, int value) {
   vfdt->maxAllocationBytes = value;
   vfdt->maxAllocationBytes *= 1024;
   vfdt->maxAllocationBytes *= 1024;
}

void VFDTSetProcessChunkSize(VFDTPtr vfdt, int value) {
   vfdt->processChunkSize = value;
}

// EEVFDT: Addded 3rd May 2017. Removed. 
// void VFDTSetProcessEfficientChunkSize(VFDTPtr vfdt, int value) {
//    gd->eChunk = value;
// }


void VFDTSetUseGini(VFDTPtr vfdt, int value) {
   vfdt->useGini = value;
}

void VFDTSetRestartLeaves(VFDTPtr vfdt, int value) {
   vfdt->reactivateLeaves = value;
}

void VFDTSetCacheTrainingExamples(VFDTPtr vfdt, int value) {
   VoidAListPtr growingNodes;
   VFDTGrowingDataPtr gd;
   int i;

   vfdt->cacheExamples = value;

   if(vfdt->cacheExamples == 0) {
      /* disable this everywhere HERE maybe enable it everywhere in the
            case where the value is 1?? */

      growingNodes = VALNew();
      DecisionTreeGatherGrowingNodes(vfdt->dtree, growingNodes);
      
      for(i = 0 ; i < VALLength(growingNodes) ; i++) {
         gd = (VFDTGrowingDataPtr)DecisionTreeGetGrowingData(VALIndex(growingNodes, i));
         if(gd->exampleCache != 0) {
            _GrowingDataFreeExampleCache(gd);
         }
      }

      VALFree(growingNodes);
   }   
}

void VFDTSetPrePruneTau(VFDTPtr vfdt, float value) {
   vfdt->prePruneTau = value;
}

void VFDTSetLaplace(VFDTPtr vfdt, int value) {
   vfdt->laplace = value;
}

void VFDTSetDoBonferonni(VFDTPtr vfdt, int value) {
   vfdt->doBonferonni = value;
}




static void _DoMakeLeaf(VFDTPtr vfdt, DecisionTreePtr currentNode) {
   VFDTGrowingDataPtr gd = ((VFDTGrowingDataPtr)DecisionTreeGetGrowingData(currentNode));
   ExampleGroupStatsPtr egs = gd->egs;
   int mostCommonClass = ExampleGroupStatsGetMostCommonClassLaplace(egs,
                    gd->parentClass, vfdt->laplace);

   vfdt->numGrowing--;
   DecisionTreeSetTypeLeaf(currentNode);

   DecisionTreeSetClass(currentNode, mostCommonClass);

   gd->seenSinceDeactivated = 0;
   gd->errorsSinceDeactivated = 0;

   ExampleGroupStatsDeactivate(egs);
   _GrowingDataFreeExampleCache(gd);
//   ExampleGroupStatsFree(egs);
//   MFreePtr(DecisionTreeGetGrowingData(currentNode));/
}
static void _DoReactivateLeaf(VFDTPtr vfdt, DecisionTreePtr currentNode) {
   VFDTGrowingDataPtr gd = ((VFDTGrowingDataPtr)DecisionTreeGetGrowingData(currentNode));
   ExampleGroupStatsPtr egs = gd->egs;

   vfdt->numGrowing++;
   DecisionTreeSetTypeGrowing(currentNode);

   ExampleGroupStatsReactivate(egs);
   if(vfdt->cacheExamples) {
      gd->exampleCache = VALNew();
   }
}

static float _CalculateReactivateIndex(DecisionTreePtr node, long totalSeen) {
   /* this is the same hueristic as the calculate leafify index but gets 
              its informationg from a slightly different place */
   float percent;
   VFDTGrowingDataPtr gd = (VFDTGrowingDataPtr)DecisionTreeGetGrowingData(node);
   float estimatedError;

   if(gd == 0) {
      /* this would happen if we've already split on attribute */
      return 0;
   }

   percent = gd->seenExamplesCount / totalSeen;

   /* use a laplace correction for the estimated error: 
          (errors + (m * parentError)) / (seen + m) */

   estimatedError = 
      (gd->errorsSinceDeactivated +
          (10 * gd->parentErrorRate)) / 
      (10 + gd->seenSinceDeactivated);

   /* return an estimate of the percentage of examples falling on 
        weighted by the accuracy the node would have if we pruned now */

   /* leaves that give more long term benefit should have higher values */
   return percent * estimatedError;
}

static float _CalculateLeafifyIndex(DecisionTreePtr node, long totalSeen) {
   float percent;
   VFDTGrowingDataPtr gd = (VFDTGrowingDataPtr)DecisionTreeGetGrowingData(node);
   float estimatedError;

   percent = gd->seenExamplesCount / totalSeen;

   /* use a laplace correction for the estimated error: 
          (errors + (m * parentError)) / (seen + m) */

   estimatedError = 
      ((ExampleGroupStatsNumExamplesSeen(gd->egs) -
          ExampleGroupStatsGetMostCommonClassCount(gd->egs)) +
          (10 * gd->parentErrorRate)) / 
      (10 + ExampleGroupStatsNumExamplesSeen(gd->egs));

   /* return an estimate of the percentage of examples falling on 
        weighted by the accuracy the node would have if we pruned now */

   /* leaves that give more long term benefit should have higher values */
   return percent * estimatedError;
}

static void _LeafifyLeastImportantNodes(VFDTPtr vfdt, long number) {
   float *indexes;
   DecisionTreePtr *nodes;
   int i, j;
   float thisIndex, tmpIndex;
   DecisionTreePtr cNode, tmpNode;
   VoidAListPtr growingNodes = VALNew();

   DecisionTreeGatherGrowingNodes(vfdt->dtree, growingNodes);

   if(vfdt->messageLevel >= 1) {
      printf("leafifying %ld of %d growing nodes\n", number, VLLength(growingNodes));
   }

   indexes = MNewPtr(sizeof(float) * number);
   nodes   = MNewPtr(sizeof(DecisionTreePtr) * number);

   /* take the first bunch of growing nodes to start */
   for(i = 0 ; i < number ; i++) {
     nodes[i] = VALIndex(growingNodes, i);
     indexes[i] = _CalculateLeafifyIndex(nodes[i], vfdt->examplesSeen);
   }

   /* EFF I should use some kind of something smart here if 
      I'm trying to prune a lot of nodes */

   /* now look at the rest of the growing nodes, if they are worse use them */
   for(i = number ; i < VALLength(growingNodes) ; i++) {
      thisIndex = _CalculateLeafifyIndex(VALIndex(growingNodes, i),
                                                      vfdt->examplesSeen);
      cNode = VALIndex(growingNodes, i);
      for(j = 0 ; j < number ; j++) {
         if(thisIndex < indexes[j]) {
            tmpIndex = indexes[j];
            indexes[j] = thisIndex;
            thisIndex = tmpIndex;
            tmpNode = nodes[j];
            nodes[j] = cNode;
            cNode = tmpNode;
         }
      }
   }

   vfdt->highestDeactivatedIndex = 0;
   /* now make them all leaves! */
   for(i = 0 ; i < number ; i++) {
      if(indexes[i] > vfdt->highestDeactivatedIndex) {
         vfdt->highestDeactivatedIndex = indexes[i];
      }
      _DoMakeLeaf(vfdt, nodes[i]);
      if(vfdt->messageLevel >= 2) {
         printf("leafifying a node with index %g\n", indexes[i]);
      }
   }

   MFreePtr(indexes);
   MFreePtr(nodes);
   VALFree(growingNodes);
}

void _FlushExampleCachesCheckStop(VFDTPtr vfdt) {
   long initialAllocation = MGetTotalAllocation();
   float *indexes;
   DecisionTreePtr *nodes;
   int i, j;
   float thisIndex, tmpIndex;
   DecisionTreePtr cNode, tmpNode;
   long finalAllocation;
   VoidAListPtr growingNodes;
   VFDTGrowingDataPtr gd;
   long number;
   long flushed;

   if(!vfdt->cacheExamples) { return; }

   growingNodes = VALNew();
   if(vfdt->messageLevel >= 2) {
      printf("flushing example cache...\n");
   }

   DecisionTreeGatherGrowingNodes(vfdt->dtree, growingNodes);

   number = VALLength(growingNodes) * .35;
   /* round up */
   number++;


   if(vfdt->messageLevel >= 2) {
      printf("thinking about flushing %ld caches\n", number);
   }

   indexes = MNewPtr(sizeof(float) * number);
   nodes   = MNewPtr(sizeof(DecisionTreePtr) * number);

   /* take the first bunch of growing nodes to start */
   for(i = 0 ; i < number ; i++) {
      nodes[i] = VALIndex(growingNodes, i);
      gd = (VFDTGrowingDataPtr)DecisionTreeGetGrowingData(nodes[i]);
      if(gd->exampleCache == 0) {
         indexes[i] = 1.0;
      } else {
         indexes[i] = _CalculateLeafifyIndex(nodes[i], vfdt->examplesSeen);
      }
   }

   /* EFF I should use some kind of something smart here if 
      I'm trying to do this to a lot of nodes */

   /* now look at the rest of the growing nodes, if they are worse use them */
   for(i = number ; i < VALLength(growingNodes) ; i++) {
      gd = (VFDTGrowingDataPtr)DecisionTreeGetGrowingData(VALIndex(
                                                growingNodes, i));
      if(gd->exampleCache == 0) {
         thisIndex = 1.0;
      } else {
         thisIndex = _CalculateLeafifyIndex(VALIndex(growingNodes, i),
                                                      vfdt->examplesSeen);
      }

      cNode = VALIndex(growingNodes, i);
      for(j = 0 ; j < number ; j++) {
         if(thisIndex < indexes[j]) {
            tmpIndex = indexes[j];
            indexes[j] = thisIndex;
            thisIndex = tmpIndex;
            tmpNode = nodes[j];
            nodes[j] = cNode;
            cNode = tmpNode;
         }
      }
   }

   flushed = 0;
   for(i = 0 ; i < number && (initialAllocation - MGetTotalAllocation() < (0.05 * vfdt->maxAllocationBytes)); i++) {
      gd = (VFDTGrowingDataPtr)DecisionTreeGetGrowingData(nodes[i]);
      if(gd->exampleCache != 0) {
         flushed++;
         _GrowingDataFreeExampleCache(gd);
      }
   }

   MFreePtr(indexes);
   MFreePtr(nodes);

   finalAllocation = MGetTotalAllocation();

   /* HERE, this .05 should be a parameter & it is used above as well */
   if(vfdt->messageLevel >= 2) {
      printf("flushed %ld freed %ld bytes stop threshold %.2f\n",
               flushed, initialAllocation - finalAllocation,
               0.05 * vfdt->maxAllocationBytes);
   }
   if(initialAllocation - finalAllocation < (0.05 * vfdt->maxAllocationBytes)) {
      /* if we don't recover at least some minimum then stop caching */
      flushed = 0;
      for(i = 0 ; i < VALLength(growingNodes) ; i++) {
         gd = (VFDTGrowingDataPtr)DecisionTreeGetGrowingData(VALIndex(growingNodes, i));
         if(gd->exampleCache != 0) {
            flushed++;
            _GrowingDataFreeExampleCache(gd);
         }
      }
      vfdt->cacheExamples = 0;
      if(vfdt->messageLevel >= 1) {
         printf("Stopped caching, there were %ld active...\n", flushed);
      }
   }

   VALFree(growingNodes);
}

static void _DoReactivateScan(VFDTPtr vfdt) {
   long number, allocation;
   float *indexes;
   DecisionTreePtr *nodes;
   int i, j;
   float thisIndex, tmpIndex;
   DecisionTreePtr cNode, tmpNode;
   VoidAListPtr leaves = VALNew();
   VFDTGrowingDataPtr gd;

   DecisionTreeGatherLeaves(vfdt->dtree, leaves);

   /* ugh, now I remove all pre-pruned nodes, this is pretty bad
       because it is expensive to remove nodes from my list */
   for(i = 0 ; i < VALLength(leaves) ; i++) {
      tmpNode = (DecisionTreePtr)VALIndex(leaves, i);
      gd = DecisionTreeGetGrowingData(tmpNode);

      if(gd->prePruned) {
         VALRemove(leaves, i);
      }
   }

   number = VALLength(leaves) * 0.05;

   if(number < 5) {
      VALFree(leaves);
      return;
   }

   if(vfdt->messageLevel >= 1) {
      printf("looking into reactivating %ld nodes\n", number);
   }

   indexes = MNewPtr(sizeof(float) * number);
   nodes   = MNewPtr(sizeof(DecisionTreePtr) * number);

   /* take the first bunch of leaves nodes to start */
   for(i = 0 ; i < number ; i++) {
     nodes[i] = VALIndex(leaves, i);
     indexes[i] = _CalculateReactivateIndex(nodes[i], vfdt->examplesSeen);
   }

   /* EFF I should use some kind of something smart here if 
      I'm trying to consider a lot of nodes */

   /* now look at the rest of the leaves, if they are better use them */
   for(i = number ; i < VALLength(leaves) ; i++) {
      thisIndex = _CalculateReactivateIndex(VALIndex(leaves, i),
                                                      vfdt->examplesSeen);
      cNode = VALIndex(leaves, i);
      for(j = 0 ; j < number ; j++) {
         if(thisIndex > indexes[j]) {
            tmpIndex = indexes[j];
            indexes[j] = thisIndex;
            thisIndex = tmpIndex;
            tmpNode = nodes[j];
            nodes[j] = cNode;
            cNode = tmpNode;
         }
      }
   }

   /* now make all of them that look good compared to our last leafify
       back into growing nodes */
   for(i = 0 ; i < number ; i++) {
      if(indexes[i] > vfdt->highestDeactivatedIndex) {
         _DoReactivateLeaf(vfdt, nodes[i]);
         if(vfdt->messageLevel >= 2) {
            printf("reactivating a node with index %f\n", indexes[i]);
         }
      }
   }

   MFreePtr(indexes);
   MFreePtr(nodes);
   VALFree(leaves);


   allocation = MGetTotalAllocation();
   if(vfdt->maxAllocationBytes < allocation) {
      if(vfdt->cacheExamples) {
         _FlushExampleCachesCheckStop(vfdt);
      } else {
         /* HERE may want to control how many to leafify with a flag */
         _LeafifyLeastImportantNodes(vfdt, vfdt->numGrowing * .2);
      }
   }
}

static void _AddExampleToGrowingNode(VFDTPtr vfdt, 
                                     DecisionTreePtr currentNode,
                                     ExamplePtr e,
                                     int checkSplits);

static float _BonferonniDelta(VFDTPtr vfdt, int numChoices) {
  // return vfdt->splitConfidence;
  // return 1 - pow(1 - vfdt->splitConfidence, 1.0 / (float)numChoices);
  //  printf("\t in bonf old confidence %f num choices %d new conf %.10f\n",
  //         vfdt->splitConfidence, numChoices, 
  //                   vfdt->splitConfidence / (float)numChoices);
   if(vfdt->doBonferonni) {
      return vfdt->splitConfidence / (float)numChoices;
   } else {
      return vfdt->splitConfidence;
   }
}


#pragma 
static void _DoContinuousSplit(VFDTPtr vfdt,
                                DecisionTreePtr currentNode, int attNum,
                                float threshold) {
   int myContCounterF = contCounterF++;
   SimMarker(1,myContCounterF);

   int i;
   AttributeTrackerPtr newAT;
   int childCount;

   VFDTGrowingDataPtr parentGD = (VFDTGrowingDataPtr)(DecisionTreeGetGrowingData(currentNode));
   AttributeTrackerPtr at =
                     ExampleGroupStatsGetAttributeTracker(parentGD->egs);
   VFDTGrowingDataPtr gd;
   long allocation;
   int parentClass;
   float parentErrorRate;
   DecisionTreePtr parent, target;
   ExamplePtr e;

   parent = currentNode;

   DebugMessage(1, 2, "  Continuous split info, attrib %s, thresh %f\n",
              ExampleSpecGetAttributeName(vfdt->spec, attNum), threshold);

   DecisionTreeSplitOnContinuousAttribute(currentNode, attNum, threshold);
   childCount = DecisionTreeGetChildCount(currentNode);

   vfdt->numGrowing--;
   vfdt->numGrowing += childCount;

   parentClass = ExampleGroupStatsGetMostCommonClass(parentGD->egs);

   parentErrorRate =
       (1.0 - (ExampleGroupStatsGetMostCommonClassCount(parentGD->egs) /
            (float)ExampleGroupStatsNumExamplesSeen(parentGD->egs)));

   /* left child, < threshold */
   newAT = AttributeTrackerClone(at);

   gd = MNewPtr(sizeof(VFDTGrowingData));
   gd->egs = ExampleGroupStatsNew(vfdt->spec, newAT);
   gd->parentClass = parentClass;
   
   gd->treeLevel = parentGD->treeLevel + 1;
   gd->prePruned = 0;
   gd->splitConfidence = _BonferonniDelta(vfdt,
                              AttributeTrackerNumActive(newAT));

   if(vfdt->cacheExamples) {
      gd->exampleCache = VALNew();
   } else {
      gd->exampleCache = 0;
   }
   gd->parentErrorRate = parentErrorRate;

   gd->seenExamplesCount = 
                  ExampleGroupStatsGetPercentBelowThreshold(parentGD->egs,
                    attNum, threshold) * parentGD->seenExamplesCount;

   gd->seenSinceLastProcess = 0;

   DecisionTreeSetGrowingData(DecisionTreeGetChild(currentNode, 0), gd);
   DecisionTreeSetClass(DecisionTreeGetChild(currentNode, 0),
                                               parentClass);
   /* right child, >= threshold */
   newAT = AttributeTrackerClone(at);

   gd = MNewPtr(sizeof(VFDTGrowingData));
   gd->egs = ExampleGroupStatsNew(vfdt->spec, newAT);
   gd->parentClass = parentClass;

   gd->treeLevel = parentGD->treeLevel + 1;
   gd->prePruned = 0;
   gd->splitConfidence = _BonferonniDelta(vfdt,
                              AttributeTrackerNumActive(newAT));

   if(vfdt->cacheExamples) {
      gd->exampleCache = VALNew();
   } else {
      gd->exampleCache = 0;
   }
   gd->parentErrorRate = parentErrorRate;

   gd->seenExamplesCount = (1.0 - 
               ExampleGroupStatsGetPercentBelowThreshold(parentGD->egs,
                    attNum, threshold)) * parentGD->seenExamplesCount;

   gd->seenSinceLastProcess = 0;

   DecisionTreeSetGrowingData(DecisionTreeGetChild(currentNode, 1), gd);
   DecisionTreeSetClass(DecisionTreeGetChild(currentNode, 1),
                                               parentClass);

   /* split the example cache */
   if(parentGD->exampleCache != 0) {
      for(i = VALLength(parentGD->exampleCache) - 1 ; i >= 0 ; i--) {
         e = VALRemove(parentGD->exampleCache, i);
         target = DecisionTreeOneStepClassify(parent, e);
         _AddExampleToGrowingNode(vfdt, target, e, 0);
      }
   }

   //_DecisionTreeWrite(currentNode);

   /* this will check to see if there is a cache first */
   _GrowingDataFreeExampleCache(parentGD);

   ExampleGroupStatsFree(parentGD->egs);
   MFreePtr(parentGD);

   DecisionTreeSetGrowingData(currentNode, 0);

   allocation = MGetTotalAllocation();
   if(vfdt->maxAllocationBytes < allocation) {
      if(vfdt->cacheExamples) {
         _FlushExampleCachesCheckStop(vfdt);
      } else {
         /* HERE may want to control how many to leafify with a flag */
         _LeafifyLeastImportantNodes(vfdt, vfdt->numGrowing * .2);
      }
   }
  SimMarker(2,myContCounterF);
}



static void _DoDiscreteSplit(VFDTPtr vfdt, DecisionTreePtr splitNode,
                             int attNum) {
  
   int myDiscreteCounterF = discreteCounterF++;
   SimMarker(1,myDiscreteCounterF);
   int i;
   AttributeTrackerPtr newAT;
   int childCount;

   VFDTGrowingDataPtr parentGD = (VFDTGrowingDataPtr)(DecisionTreeGetGrowingData(splitNode));
   AttributeTrackerPtr at =
                     ExampleGroupStatsGetAttributeTracker(parentGD->egs);
   VFDTGrowingDataPtr gd;
   long allocation;
   int parentClass;
   float parentErrorRate;
   DecisionTreePtr target;
   ExamplePtr e;

   parentClass = ExampleGroupStatsGetMostCommonClass(parentGD->egs);

   if(AttributeTrackerAreAllInactive(at)) {
      /* made a split on every attribute along this path so don't try
          to split any more */
      DecisionTreeSetTypeLeaf(splitNode);
      DecisionTreeSetClass(splitNode, parentClass);
   } else {
      DecisionTreeSplitOnDiscreteAttribute(splitNode, attNum);
      childCount = DecisionTreeGetChildCount(splitNode);

      vfdt->numGrowing--;
      vfdt->numGrowing += childCount;

      parentErrorRate =
          (1.0 - (ExampleGroupStatsGetMostCommonClassCount(parentGD->egs) /
               (float)ExampleGroupStatsNumExamplesSeen(parentGD->egs)));

      for(i = 0 ; i < childCount ; i++) {
         newAT = AttributeTrackerClone(at);
         AttributeTrackerMarkInactive(newAT, attNum);

         gd = MNewPtr(sizeof(VFDTGrowingData));
         gd->egs = ExampleGroupStatsNew(vfdt->spec, newAT);
         gd->parentClass = parentClass;

         gd->treeLevel = parentGD->treeLevel + 1;
         gd->prePruned = 0;
         gd->splitConfidence = _BonferonniDelta(vfdt,
                     AttributeTrackerNumActive(newAT));

         if(vfdt->cacheExamples) {
            gd->exampleCache = VALNew();
         } else {
            gd->exampleCache = 0;
         }
         gd->parentErrorRate = parentErrorRate;

         gd->seenExamplesCount =
                 ExampleGroupStatsGetValuePercent(parentGD->egs,
                          attNum, i) * parentGD->seenExamplesCount;

         gd->seenSinceLastProcess = 0;

         DecisionTreeSetGrowingData(DecisionTreeGetChild(splitNode, i), gd);
         DecisionTreeSetClass(DecisionTreeGetChild(splitNode, i),
                                                  parentClass);
      }

      /* split the example cache */
      if(parentGD->exampleCache != 0) {
         for(i = VALLength(parentGD->exampleCache) - 1 ; i >= 0 ; i--) {
            e = VALRemove(parentGD->exampleCache, i);
            target = DecisionTreeOneStepClassify(splitNode, e);
            _AddExampleToGrowingNode(vfdt, target, e, 0);
         }
      }
   }

   /* this will check to see if there is a cache first */
   _GrowingDataFreeExampleCache(parentGD);

   ExampleGroupStatsFree(parentGD->egs);
   MFreePtr(parentGD);

   DecisionTreeSetGrowingData(splitNode, 0);

   allocation = MGetTotalAllocation();
   if(vfdt->maxAllocationBytes < allocation) {
      if(vfdt->cacheExamples) {
         _FlushExampleCachesCheckStop(vfdt);
      } else {
         /* HERE may want to control how many to leafify with a flag */
         _LeafifyLeastImportantNodes(vfdt, vfdt->numGrowing * .2);
      }
   }
   SimMarker(2,myDiscreteCounterF);

}

static void _CheckSplit(VFDTPtr vfdt, DecisionTreePtr currentNode, int forceSplit) {
   int myCheckSplitCounterF = checkSplitCounterF++;
   SimMarker(1,myCheckSplitCounterF);
   
   float hoeffdingBound;
   int i;
   float range;
   float *allIndexes;
   float firstIndex, secondIndex, nullIndex;
   int firstAttrib, secondAttrib;
   float firstThresh, secondThresh;
   float tmpIndexOne, tmpIndexTwo, tmpThreshOne, tmpThreshTwo;
   VFDTGrowingDataPtr gd = DecisionTreeGetGrowingData(currentNode);
   ExampleGroupStatsPtr egs = gd->egs;

   int myBestAttrCounter = bestAttrCounter++;
   SimMarker(1,myBestAttrCounter);
   /* count the bounds that need to hold */
   vfdt->numBoundsUsed += 
        AttributeTrackerNumActive(ExampleGroupStatsGetAttributeTracker(egs));

   firstIndex = secondIndex = 10000;
   firstAttrib = secondAttrib = -1;
   firstThresh = secondThresh = 0;
   allIndexes =
          MNewPtr(sizeof(float) * ExampleSpecGetNumAttributes(vfdt->spec));
   for(i = 0 ; i < ExampleSpecGetNumAttributes(vfdt->spec) ; i++) {
      if(ExampleSpecIsAttributeDiscrete(vfdt->spec, i)) {
         /* set up the second tmp index to look really bad (unless updated by
              a continuous attribute to look better) */
         tmpIndexTwo = 10000;
         if(vfdt->useGini) {
            tmpIndexOne = ExampleGroupStatsGiniDiscreteAttributeSplit(egs, i);
         } else {
            tmpIndexOne = 
                     ExampleGroupStatsEntropyDiscreteAttributeSplit(egs, i);
         }
      } else { /* continuous attribute */
         if(vfdt->useGini) {
            ExampleGroupStatsGiniContinuousAttributeSplit(egs, i,
                   &tmpIndexOne, &tmpThreshOne, &tmpIndexTwo, &tmpThreshTwo);
         } else {
            ExampleGroupStatsEntropyContinuousAttributeSplit(egs, i,
                   &tmpIndexOne, &tmpThreshOne, &tmpIndexTwo, &tmpThreshTwo);
         }
      }

      allIndexes[i] = tmpIndexOne;

      if(tmpIndexOne < firstIndex) {
         /* bump the first one over */
         secondIndex = firstIndex;
         secondThresh = firstThresh;
         secondAttrib = firstAttrib;

         /* replace the first one */
         firstIndex = tmpIndexOne;
         firstThresh = tmpThreshOne;
         firstAttrib = i;

         if(tmpIndexTwo < secondIndex) {
            /* replace the second one */
            secondIndex = tmpIndexTwo;
            secondThresh = tmpThreshTwo;
            secondAttrib = i;
         }
      } else if(tmpIndexOne < secondIndex) {
         /* replace the second one */
         secondIndex = tmpIndexOne;
         secondThresh = tmpThreshOne;
         secondAttrib = i;

         /* the tmpIndexTwo must be worse than what we got so ignore it */
      }
   }
   if(vfdt->messageLevel >= 3) {
      printf("Considering split, best two indecies are:\n");
      printf("   Index: %f Thresh: %f Attrib: %d\n", firstIndex, firstThresh, firstAttrib);
      printf("   Index: %f Thresh: %f Attrib: %d\n", secondIndex, secondThresh, secondAttrib);
   }

   if(vfdt->useGini) {
      range = 1.0;
   } else {
      range = lg(ExampleSpecGetNumClasses(vfdt->spec));
   }

   /* HERE, this will crash if splitConfidence is 0 */
   hoeffdingBound = sqrt(((range * range) * log(1/gd->splitConfidence)) / 
       (2.0 * ExampleGroupStatsNumExamplesSeen(egs)));
   SimMarker(2,myBestAttrCounter);
   
   /* check for prepruning */
   if(vfdt->useGini) {
      nullIndex = ExampleGroupStatsGiniTotal(egs);
   } else {
      nullIndex = ExampleGroupStatsEntropyTotal(egs);
   }

   int myPrepruneCounter = prepruneCounter++;
   SimMarker(1,myPrepruneCounter);
   if(firstIndex - nullIndex > hoeffdingBound) {
      if(vfdt->messageLevel >= 1) {
         printf("Prepruned based on a null-win over %d, index %f, hoeffding %f, based on %ld examples, null index %f, level %d\n",
               firstAttrib, firstIndex, 
               hoeffdingBound, ExampleGroupStatsNumExamplesSeen(egs),
               nullIndex, gd->treeLevel);
      }

      gd->prePruned = 1;
   } else if((nullIndex - firstIndex < hoeffdingBound) && 
                         hoeffdingBound < vfdt->prePruneTau) {
      if(vfdt->messageLevel >= 1) {
         printf("Prepruned based on a null tie with %d, index %f, hoeffding %f, based on %ld examples, null index %f, level %d\n",
               firstAttrib, firstIndex, 
               hoeffdingBound, ExampleGroupStatsNumExamplesSeen(egs),
               nullIndex, gd->treeLevel);
      }
      gd->prePruned = 1;
   }
   SimMarker(2,myPrepruneCounter);
   

   if(gd->prePruned) {
      int myMakeLeafCounter = makeLeafCounter++;
      SimMarker(1,myMakeLeafCounter);
      _DoMakeLeaf(vfdt, currentNode);
      SimMarker(2,myMakeLeafCounter);
      
   }

   if(!gd->prePruned) {
      /* if we win by the hoeffding test 
         or
         if we are in forceSplit mode and it's a good time to split */
      

      if((secondIndex - firstIndex > hoeffdingBound)
                || 
         (forceSplit && ExampleGroupStatsNumExamplesSeen(egs) > 5 &&
          !ExampleGroupStatsIsPure(egs))) {
                 //&& ExampleGroupStatsEntropyTotal(egs) > firstIndex)) {
          
         int myInsidePrePruneCounter = insidePrePruneCounter++;
         SimMarker(1,myInsidePrePruneCounter);
         if(vfdt->messageLevel >= 1) {
            if(ExampleSpecIsAttributeDiscrete(vfdt->spec, firstAttrib)) {
               printf("Did discrete split on attrib %s, index %f, second best %s, index %f, hoeffding %f, based on %ld examples, presplit entropy %f, level %d\n",
                     ExampleSpecGetAttributeName(vfdt->spec, firstAttrib), 
                         firstIndex, 
                     ExampleSpecGetAttributeName(vfdt->spec, secondAttrib),
                     secondIndex, hoeffdingBound,
                     ExampleGroupStatsNumExamplesSeen(egs),
                     ExampleGroupStatsEntropyTotal(egs),
                     gd->treeLevel);
            } else {
               printf("Did continuous split on attrib %s, thresh %f, index %f, second best %s, thresh %f, index %f, hoeffding %f, based on %ld examples, presplit entropy %f, level %d\n",
		   ExampleSpecGetAttributeName(vfdt->spec, firstAttrib),
                     firstThresh, firstIndex, 
                     ExampleSpecGetAttributeName(vfdt->spec, secondAttrib),
                     secondThresh, secondIndex, hoeffdingBound,
                     ExampleGroupStatsNumExamplesSeen(egs),
                     ExampleGroupStatsEntropyTotal(egs),
                     gd->treeLevel);
            }
            if(vfdt->messageLevel >= 2) {
               printf("   allocation now: %ld\n", MGetTotalAllocation());
            }
         }

         if(vfdt->messageLevel >= 4) {
            ExampleGroupStatsWrite(egs, stdout);
         }
         SimMarker(2,myInsidePrePruneCounter);
         
         int myDiscreteCounter = discreteCounter++;
         int myContCounter = contCounter++;

         if(ExampleSpecIsAttributeDiscrete(vfdt->spec, firstAttrib)) {
            // SimMarker(1,myDiscreteCounter);
            _DoDiscreteSplit(vfdt, currentNode, firstAttrib);
            // SimMarker(2,myDiscreteCounter);

         } else {
            // SimMarker(1,myContCounter);
            _DoContinuousSplit(vfdt, currentNode, firstAttrib, firstThresh);
            // SimMarker(2,myContCounter);
         }

         if(vfdt->messageLevel >= 4) {
            printf("The new tree is: \n");
            DecisionTreePrint(vfdt->dtree, stdout);
         }

      // EEFVDT: 3rd May I think nullIndex here is incorrect 

      } else if(hoeffdingBound < vfdt->tieConfidence && 
                 nullIndex - firstIndex > vfdt->prePruneTau) {
         /* if it looks like we have a tie */
         if(vfdt->messageLevel >= 1) {
            printf("Called it a tie between attrib %d, index %f, and %d, index %f, hoeffding %f, based on %ld examples, presplit entropy %f, level %d\n",
                  firstAttrib, firstIndex, secondAttrib, secondIndex,
                  hoeffdingBound, ExampleGroupStatsNumExamplesSeen(egs),
                  ExampleGroupStatsEntropyTotal(egs),
                  gd->treeLevel);
            if(vfdt->messageLevel >= 2) {
               printf("   allocation now: %ld\n", MGetTotalAllocation());
            }
         }
         
         // int myDiscreteCounter = discreteCounter++;
         // int myContCounter = contCounter++;
         if(ExampleSpecIsAttributeDiscrete(vfdt->spec, forirstAttrib)) {
            // SimMarker(1,myDiscreteCounter);
            _DoDiscreteSplit(vfdt, currentNode, firstAttrib);
            // SimMarker(2,myDiscreteCounter);
         } else {
            // SimMarker(1,myContCounter);
            _DoContinuousSplit(vfdt, currentNode, firstAttrib, firstThresh);
            // SimMarker(2,myContCounter);
         }
      } else {
         // Not splitting. 
         // EEVFDT 3rd may: If not split, update eChunk. eChunk is going to be the n so that epsilon=deltaG 

         // EEVFDT 8th May. If attributes are too similar, nmin is going to be controlled by tau 

         if ((secondIndex - firstIndex) <= vfdt->tieConfidence) {
                gd->eChunk = (range * range)* log(1/gd->splitConfidence) / 
            (2.0 * (vfdt->tieConfidence)*(vfdt->tieConfidence) );
         }else{
             gd->eChunk = (range * range)* log(1/gd->splitConfidence) / 
            (2.0 * (secondIndex - firstIndex)*(secondIndex - firstIndex) );
         }

        


   // hoeffdingBound = sqrt(((range * range) * log(1/gd->splitConfidence)) / 
   //     (2.0 * ExampleGroupStatsNumExamplesSeen(egs)));

         if(vfdt->messageLevel >= 2) {
            printf("Didn't split, top indexes were %f at %d and %f at %d hoeffding %f\n",
                     firstIndex, firstAttrib, secondIndex, secondAttrib,
                       hoeffdingBound);
            //ExampleGroupStatsWrite(egs, stdout);
         }

         if(vfdt->messageLevel >= 2) {
            printf("Settin g a new chunk value:  %d, with differences between attributes: %f. Hoeffding %f \n",
                     gd->eChunk, secondIndex - firstIndex, hoeffdingBound );
            //ExampleGroupStatsWrite(egs, stdout);
         }


         /* now disable any that aren't promising */
         int myDisableLeafCounter = disableLeafCounter++;
         SimMarker(1,myDisableLeafCounter);
         
         for(i = 0 ; i < ExampleSpecGetNumAttributes(vfdt->spec) ; i++) {
            if(allIndexes[i] > firstIndex + hoeffdingBound
                   && i != firstAttrib && i != secondAttrib) {
               ExampleGroupStatsIgnoreAttribute(egs, i);
            
               if(vfdt->messageLevel >= 3) {
                  printf("started ignoring an attribute %d index %f, best %f\n based on %ld examples\n", i, allIndexes[i], firstIndex, ExampleGroupStatsNumExamplesSeen(egs));
               }
            } else if(!ExampleSpecIsAttributeDiscrete(vfdt->spec, i)) {
               /* start ignoring bad split points */
               if(vfdt->useGini) {
                  ExampleGroupStatsIgnoreSplitsWorseThanGini(egs,
					    i, firstIndex + hoeffdingBound);
               } else {
                  ExampleGroupStatsIgnoreSplitsWorseThanEntropy(egs,
					    i, firstIndex + hoeffdingBound);
               }
	        }
         }
         
         SimMarker(2,myDisableLeafCounter);
      }
   }
   MFreePtr(allIndexes);
   SimMarker(2,myCheckSplitCounterF);

}

static void _AddExampleToGrowingNode(VFDTPtr vfdt, 
                                     DecisionTreePtr currentNode,
                                     ExamplePtr e,
                                     int checkSplits) {

   int myAddExampleGrowingCounterF = addExampleGrowingCounterF++;
   SimMarker(1, myAddExampleGrowingCounterF);
   int i;
   int myUpdateStatsCounter = updateStatsCounter++;
   SimMarker(1,myUpdateStatsCounter);
   VFDTGrowingDataPtr gd = (VFDTGrowingDataPtr)DecisionTreeGetGrowingData(currentNode);
   ExampleGroupStatsPtr egs = gd->egs;

   /* add the information */
   ExampleGroupStatsAddExample(egs, e);

   for(i = 0 ; i < ExampleSpecGetNumAttributes(vfdt->spec) ; i++) {
      if(ExampleSpecIsAttributeContinuous(vfdt->spec, i)) {
         if(ExampleGroupStatsIsAttributeActive(egs, i)) {
            if(ExampleGroupStatsNumSplitThresholds(egs, i) > 1000) {
               ExampleGroupStatsStopAddingSplits(egs, i);
            }
         }
      }
   }
   SimMarker(2,myUpdateStatsCounter);


   if(vfdt->cacheExamples && gd->exampleCache != 0) {
      VALAppend(gd->exampleCache, e);
   } else {
      ExampleFree(e);
   }

   gd->seenSinceLastProcess++;
   gd->seenExamplesCount++;
   
   // EEVFDT 3rd May
   if((gd->seenSinceLastProcess >= gd->eChunk) && checkSplits) {

   // if((gd->seenSinceLastProcess >= vfdt->processChunkSize) && checkSplits) {
      gd->seenSinceLastProcess = 0;
      if(!ExampleGroupStatsIsPure(egs)) {
         int myCheckSplitCounter = checkSplitCounter++;
         // SimMarker(1,  myCheckSplitCounter) ;
         _CheckSplit(vfdt, currentNode, 0);
         // SimMarker(2,myCheckSplitCounter);
      }
	}
     SimMarker(2, myAddExampleGrowingCounterF);
}

static void _AddExampleToDeactivatedNode(VFDTPtr vfdt, 
                                     DecisionTreePtr currentNode,
                                     ExamplePtr e) {
   VFDTGrowingDataPtr gd = (VFDTGrowingDataPtr)DecisionTreeGetGrowingData(currentNode);

   if(gd != 0) {
      /* if it's zero we've already spilt on every attribute on that path */
      gd->seenSinceDeactivated++;

      if(ExampleGetClass(e) != DecisionTreeGetClass(currentNode)) {
         gd->errorsSinceDeactivated++;
      }
   }

   ExampleFree(e);

}

static void _ProcessExampleBatch(VFDTPtr vfdt, ExamplePtr e) {
   int foundNode = 0;
   DecisionTreePtr currentNode;

   currentNode = vfdt->dtree;

   while(!foundNode && currentNode != 0) {
      if(DecisionTreeIsLeaf(currentNode)) {
         /* We are done growing in this direction, this example isn't needed */
         foundNode = 1;
      } else if(DecisionTreeIsNodeGrowing(currentNode)) {
         foundNode = 1;
      }

      if(!foundNode) {
         currentNode = DecisionTreeOneStepClassify(currentNode, e);
      }
   }
      
   if(!DecisionTreeIsLeaf(currentNode)) { /* if leaf ignore the example */
      /* here, this doesn't check for splits */
      _AddExampleToGrowingNode(vfdt, currentNode, e, 0);
   } else {
      _AddExampleToDeactivatedNode(vfdt, currentNode, e);
      if(vfdt->messageLevel >= 3) {
         printf("not learning from:");
         ExampleWrite(e, stdout);
      }
   }

   if(vfdt->examplesSeen % vfdt->reactivateScanPeriod == 0) {
      _DoReactivateScan(vfdt);
   }

   if(vfdt->maxAllocationBytes < MGetTotalAllocation()) {
      if(vfdt->cacheExamples) {
         _FlushExampleCachesCheckStop(vfdt);
      } else {
         /* HERE may want to control how many to leafify with a flag */
         _LeafifyLeastImportantNodes(vfdt, vfdt->numGrowing * .2);
      }
   }

   if((vfdt->messageLevel >= 1) && (vfdt->examplesSeen % 1000 == 0)) {
      printf("processed %ld examples\n", vfdt->examplesSeen);
   }
}


void _forceSplits(VFDTPtr vfdt, DecisionTreePtr dt) {
   int i;

   if(DecisionTreeIsLeaf(dt) || DecisionTreeIsNodeGrowing(dt)) {
      _CheckSplit(vfdt, dt, 1);
   }
   
   if(!DecisionTreeIsLeaf(dt) && !DecisionTreeIsNodeGrowing(dt)) {
      for(i = 0 ; i < DecisionTreeGetChildCount(dt) ; i++) {
         _forceSplits(vfdt, DecisionTreeGetChild(dt, i));
      }
   }
}


static void _PrintSpaces(FILE *out, int num) {
   int i;

   for(i = 0 ; i < num ; i++) {
      fprintf(out, " ");
   }
}

static void _PrintHelp(DecisionTreePtr dt, FILE *out, int indent) {
   int i;
   VFDTGrowingDataPtr gd;

   _PrintSpaces(out, indent);
   if(dt->nodeType == dtnLeaf || dt->nodeType == dtnGrowing) {
     //fprintf(out, "(leaf: %d)\n", dt->myclass);
      fprintf(out, "(leaf: %s -", 
             ExampleSpecGetClassValueName(dt->spec, dt->myclass));
      gd = DecisionTreeGetGrowingData(dt);
      for(i = 0 ; i < ExampleSpecGetNumClasses(dt->spec) ; i++) {
         fprintf(out, " %ld", gd->egs->classTotals[i]);
      }
      fprintf(out, ")\n");
   } else if(dt->nodeType == dtnDiscrete) {
      fprintf(out, "(split on %s:\n",
             ExampleSpecGetAttributeName(dt->spec, dt->splitAttribute));
      for(i = 0 ; i < VALLength(dt->children) ; i++) {
         _PrintSpaces(out, indent + 1);
         fprintf(out, "%s\n",
          ExampleSpecGetAttributeValueName(dt->spec, dt->splitAttribute, i));
         _PrintHelp(VALIndex(dt->children, i), out, indent + 2);
      }
      _PrintSpaces(out, indent);
      fprintf(out, ")\n");
   } else if(dt->nodeType == dtnContinuous) {
      fprintf(out, "(split on %s:\n",
             ExampleSpecGetAttributeName(dt->spec, dt->splitAttribute));

      /* left child */
      _PrintSpaces(out, indent + 1);
      fprintf(out, "< %f\n", dt->splitThreshold);
      _PrintHelp(VALIndex(dt->children, 0), out, indent + 2);

      /* right child */
      _PrintSpaces(out, indent + 1);
      fprintf(out, ">= %f\n", dt->splitThreshold);
      _PrintHelp(VALIndex(dt->children, 1), out, indent + 2);


      _PrintSpaces(out, indent);
      fprintf(out, ")\n");
   } else if(dt->nodeType == dtnGrowing) {
      fprintf(out, "(growing)\n");
   }
}

static void _DecisionTreeWrite(DecisionTreePtr dt) {
   _PrintHelp(dt, stdout, 0);
}

void _ProcessExamplesBatch(VFDTPtr vfdt, FILE *input) {
   ExamplePtr e;

   e = ExampleRead(input, vfdt->spec);

   while(e != 0 && vfdt->numGrowing > 0) {
      _ProcessExampleBatch(vfdt, e);
      vfdt->examplesSeen++;

      e = ExampleRead(input, vfdt->spec);
   }

   _forceSplits(vfdt, vfdt->dtree);

   //_DecisionTreeWrite(vfdt->dtree);

   if(vfdt->messageLevel >= 1) {
      printf("finished with all the examples, there are %ld growing nodes\n",
              vfdt->numGrowing);
   }
}


// #pragma GCC push_options
// #pragma GCC optimize ("00")
static void _ProcessExample(VFDTPtr vfdt, ExamplePtr e) {
   int myProcessExCounterF = processExCounterF++;
   SimMarker(1,myProcessExCounterF);
   //processExCounterF++;
   //SimMarker(1,processExCounterF);
   int foundNode = 0;
   DecisionTreePtr currentNode;

   currentNode = vfdt->dtree;
   int myFilterInstanceCounter = filterInstanceCounter++;
   SimMarker(1,myFilterInstanceCounter);
   while(!foundNode && currentNode != 0) {
      if(DecisionTreeIsLeaf(currentNode)) {
         /* We are done growing in this direction, this example isn't needed */
         foundNode = 1;
      } else if(DecisionTreeIsNodeGrowing(currentNode)) {
         foundNode = 1;
      }

      if(!foundNode) {
         currentNode = DecisionTreeOneStepClassify(currentNode, e);
      }
   }
   SimMarker(2,myFilterInstanceCounter);

   if(!DecisionTreeIsLeaf(currentNode)) { /* if leaf ignore the example */
      int myAddExampleGrowingCounter = addExampleGrowingCounter++;
      // SimMarker(1,myAddExampleGrowingCounter);

      _AddExampleToGrowingNode(vfdt, currentNode, e, 1);
      
      // SimMarker(2,myAddExampleGrowingCounter);
   } else {
      _AddExampleToDeactivatedNode(vfdt, currentNode, e);
      if(vfdt->messageLevel >= 3) {
         printf("not learning from:");
         ExampleWrite(e, stdout);
      }
   }

   if(vfdt->examplesSeen % vfdt->reactivateScanPeriod == 0) {
      int myReactivateScancounter = reactivateScancounter++;

      SimMarker(1,myReactivateScancounter);
      _DoReactivateScan(vfdt);
      SimMarker(2,myReactivateScancounter);
   }

   if(vfdt->maxAllocationBytes < MGetTotalAllocation()) {
      if(vfdt->cacheExamples) {
         int myFlushCounter=flushCounter++;
         
         SimMarker(1,myFlushCounter);
         _FlushExampleCachesCheckStop(vfdt);
         SimMarker(2,myFlushCounter);

      } else {
         /* HERE may want to control how many to leafify with a flag */
         int myLeafifyCounter = leafifyCounter++;

         SimMarker(1,myLeafifyCounter);
         _LeafifyLeastImportantNodes(vfdt, vfdt->numGrowing * .2);
         SimMarker(2,myLeafifyCounter);
      }
   }
   SimMarker(2,myProcessExCounterF);
}

// #pragma GCC pop_options

void _ProcessExamples(VFDTPtr vfdt, FILE *input) {
   ExamplePtr e;

   e = ExampleRead(input, vfdt->spec);

   while(e != 0 && vfdt->numGrowing > 0) {
      _ProcessExample(vfdt, e);
      vfdt->examplesSeen++;
      if((vfdt->messageLevel >= 1) && (vfdt->examplesSeen % 1000 == 0)) {
         printf("processed %ld examples\n", vfdt->examplesSeen);
      }

      e = ExampleRead(input, vfdt->spec);
   }

   if(vfdt->messageLevel >= 1) {
      printf("finished with all the examples, there are %ld growing nodes\n",
              vfdt->numGrowing);
   }
}

//void VFDTBootstrapC45(VFDTPtr vfdt, char *fileStem, int overprune, int runC45) {
//   char command[500], fileName[100];
//   FILE *tree;

//   if(runC45) {
//      sprintf(command, "c4.5 -f %s -u >> /dev/null", fileStem);
//      if(vfdt->messageLevel >= 1) {
//         printf("%s\n", command);
//      }
//      system(command);
//   }

//   DecisionTreeFree(vfdt->dtree);

//   sprintf(fileName, "%s.tree", fileStem);
//   tree = fopen(fileName, "r");
//   if(overprune) {
//      vfdt->dtree = DecisionTreeReadC45Overprune(tree, vfdt->spec);
//   } else {
//      vfdt->dtree = DecisionTreeReadC45(tree, vfdt->spec);
//   }
//   fclose(tree);

//   /* now reactivate the thing */
//   _ReactivateLeaves(vfdt);
//}

void VFDTProcessExamples(VFDTPtr vfdt, FILE *input) {
   _ProcessExamples(vfdt, input);
}

void VFDTProcessExampleBatch(VFDTPtr vfdt, ExamplePtr e) {
   _ProcessExampleBatch(vfdt, e);
}

void VFDTBatchExamplesDone(VFDTPtr vfdt) {
   vfdt->batchMode = 1;
   _forceSplits(vfdt, vfdt->dtree);
}

void VFDTProcessExamplesBatch(VFDTPtr vfdt, FILE *input) {
   vfdt->batchMode = 1;
   _ProcessExamplesBatch(vfdt, input);
}

void VFDTProcessExample(VFDTPtr vfdt, ExamplePtr e) {
   vfdt->examplesSeen++;
   _ProcessExample(vfdt, e);
}


int VFDTIsDoneLearning(VFDTPtr vfdt) {
   return !DecisionTreeIsTreeGrowing(vfdt->dtree);
}

long VFDTGetNumGrowing(VFDTPtr vfdt) {
   return vfdt->numGrowing;
}

long VFDTGetNumBoundsUsed(VFDTPtr vfdt) {
   return vfdt->numBoundsUsed;
}


void VFDTPrintStats(VFDTPtr vfdt, FILE *out) {
   int i;
   VoidAListPtr list = VALNew();
   ExampleGroupStatsPtr egs;
   DecisionTreePtr growNode;
   long seenSum = 0;
   long commonCountSum = 0;
   long pureCount = 0;

   DecisionTreeGatherGrowingNodes(vfdt->dtree, list);

   for(i = VALLength(list) - 1 ; i >= 0 ; i--) {
      growNode = VALIndex(list, i);
      egs = ((VFDTGrowingDataPtr)DecisionTreeGetGrowingData(growNode))->egs;

      seenSum += ExampleGroupStatsNumExamplesSeen(egs);
      commonCountSum += ExampleGroupStatsGetMostCommonClassCount(egs);
      if(ExampleGroupStatsIsPure(egs)) {
         pureCount++;
      }
   }

   if(VALLength(list) > 0) {
      fprintf(out, "growing - seen %ld ave %ld - avg mcc %ld - pure %ld\n", seenSum, seenSum / VALLength(list), commonCountSum / VALLength(list), pureCount);
   } else {
      fprintf(out, "There aren't any growing nodes\n");
   }

   VALFree(list);

   fprintf(out, "used %ld bounds checks\n", vfdt->numBoundsUsed);

   DecisionTreePrintStats(vfdt->dtree, out);
}

DecisionTreePtr VFDTGetLearnedTree(VFDTPtr vfdt) {
   int i;
   VoidAListPtr list = VALNew();
   ExampleGroupStatsPtr egs;
   DecisionTreePtr growNode;
   DecisionTreePtr finalTree = DecisionTreeClone(vfdt->dtree);

   DecisionTreeGatherGrowingNodes(finalTree, list);

   for(i = VALLength(list) - 1 ; i >= 0 ; i--) {
      growNode = VALIndex(list, i);

      DecisionTreeSetTypeLeaf(growNode);
      egs = ((VFDTGrowingDataPtr)DecisionTreeGetGrowingData(growNode))->egs;

      if(!vfdt->batchMode) {
         /* only use the laplace if we aren't in batch mode */
         DecisionTreeSetClass(growNode, 
          ExampleGroupStatsGetMostCommonClassLaplace(egs,
           ((VFDTGrowingDataPtr)DecisionTreeGetGrowingData(growNode))->parentClass, 5));
      } else {
         DecisionTreeSetClass(growNode, 
          ExampleGroupStatsGetMostCommonClass(egs));
      }
   }

   VALFree(list);

   return finalTree;
}

//static float _TestAccuracy(DecisionTreePtr dt, VoidAListPtr examples) {
//   long mistakes = 0;
//   ExamplePtr e;
//   int class;
//   int i;

//   for(i = 0 ; i < VALLength(examples) ; i++) {
//      e = VALIndex(examples, i);
//      class = DecisionTreeClassify(dt, e);
//      if(class != ExampleGetClass(e)) {
//         mistakes++;
//      }
//   }

//   return 1.0 - ((float)mistakes / (float)VALLength(examples));
//}

//float _bestPruneAccuracy;
//DecisionTreePtr _bestPruneNode;

//static void _REPrune(DecisionTreePtr dt, DecisionTreePtr currentNode, 
//                          VoidAListPtr examples) {
//   NodeType oldNodeType;
//   int oldNodeClass;
//   float newAccuracy;
//   int i;

//   if(currentNode->nodeType == dtnGrowing || currentNode->nodeType == dtnLeaf) {
//      return;
//   } else {
//      oldNodeType = currentNode->nodeType;
//      oldNodeClass = currentNode->class;

//      currentNode->class = DecisionTreeGetMostCommonClass(currentNode);
//      currentNode->nodeType = dtnLeaf;

//      newAccuracy = _TestAccuracy(dt, examples);
//      if(newAccuracy >= _bestPruneAccuracy) {
//         _bestPruneAccuracy = newAccuracy;
//         _bestPruneNode = currentNode;
//      }
//      currentNode->nodeType = oldNodeType;
//      currentNode->class = oldNodeClass;
//      for(i = 0 ; i < VALLength(currentNode->children) ; i++) {
//         _REPrune(dt, VALIndex(currentNode->children, i), examples);
//      }
//   }
//}

/* HERE I could split the examples as I recur, but...I'm not... */
//void VFDTREPrune(DecisionTreePtr dt, VoidAListPtr examples) {
//   float currentAccuracy;
//   int progress = 1;
//   int i;

//   while(progress) {
//      _bestPruneAccuracy = 0;
//      _bestPruneNode = 0;

//      _REPrune(dt, dt, examples);

//      if(_bestPruneNode == 0 ||
//        (_bestPruneAccuracy < _TestAccuracy(dt, examples))) {
//       /* if there is nothing to prune or pruning makes things worse */
//         progress = 0;
//      } else {
//         _bestPruneNode->class = DecisionTreeGetMostCommonClass(_bestPruneNode);
//         _bestPruneNode->nodeType = dtnLeaf;

//         for(i = 0 ; i < VALLength(_bestPruneNode->children) ; i++) {
//            DecisionTreeFree(VALIndex(_bestPruneNode->children, i));
//         }
//         VALFree(_bestPruneNode->children);
//      }
//   }
//}

typedef struct _PRUNEDATA_ {
   long errors;
   int class;
   int errorDelta;
   DecisionTreePtr parent;
} PruneData, *PruneDataPtr;


static void _InitPruneData(DecisionTreePtr current, DecisionTreePtr parent) {
   PruneDataPtr data;
   long i;

   data = MNewPtr(sizeof(PruneData));
   data->errors = 0;
   data->class = DecisionTreeGetMostCommonClass(current);
   data->errorDelta = 0;
   data->parent = parent;

   DecisionTreeSetGrowingData(current, data);
   if(current->nodeType != dtnGrowing && current->nodeType != dtnLeaf) {
      for(i = 0 ; i < VALLength(current->children) ; i++) {
         _InitPruneData(VALIndex(current->children, i), current);
      }
   }
}

static void _UpdateErrorCounts(DecisionTreePtr current, ExamplePtr e) {
   PruneDataPtr data = DecisionTreeGetGrowingData(current);


   if(data->class != ExampleGetClass(e)) {
      data->errors += 1;
      data->errorDelta += 1;
   }

   if(current->nodeType != dtnGrowing && current->nodeType != dtnLeaf) {
      _UpdateErrorCounts(DecisionTreeOneStepClassify(current, e), e);
   }
}

static long _InitErrorDelta(DecisionTreePtr current) {
   PruneDataPtr data = DecisionTreeGetGrowingData(current);
   long sum = 0;
   long i;

   if(current->nodeType != dtnGrowing && current->nodeType != dtnLeaf) {
      for(i = 0 ; i < VALLength(current->children) ; i++) {
         sum += _InitErrorDelta(VALIndex(current->children, i));
      }

      data->errorDelta -= sum;
      return sum;
   } else {
      return data->errors;
   }
}

static void _FreePruneData(DecisionTreePtr dt) {
   long i;

   MFreePtr(DecisionTreeGetGrowingData(dt));
   if(dt->nodeType != dtnGrowing && dt->nodeType != dtnLeaf) {
      for(i = 0 ; i < VALLength(dt->children) ; i++) {
         _FreePruneData(VALIndex(dt->children, i));
      }
   }
}

static DecisionTreePtr _FindBestPruneNode(DecisionTreePtr dt) {
   PruneDataPtr bestData = DecisionTreeGetGrowingData(dt);
   DecisionTreePtr best = dt;
   PruneDataPtr tmpData;
   DecisionTreePtr tmp;
   long i;

   if(dt->nodeType != dtnGrowing && dt->nodeType != dtnLeaf) {
      for(i = 0 ; i < VALLength(dt->children) ; i++) {
         tmp = _FindBestPruneNode(VALIndex(dt->children, i));
         if(tmp != 0) {
            tmpData = DecisionTreeGetGrowingData(tmp);
            if(tmpData->errorDelta < bestData->errorDelta) {
               best = tmp;
               bestData = tmpData;
            }
         }
      }

      if(bestData->errorDelta <= 0) {
         return best;
      }
   }

   return 0;
}

static void _PrintPruneData(DecisionTreePtr dt, int level) {
   PruneDataPtr bestData = DecisionTreeGetGrowingData(dt);
   long i;

   for(i = 0 ; i < level ; i++) {
      printf(" ");
   }
   printf("l%d e%ld d%d c%d\n", level, 
        bestData->errors, bestData->errorDelta, bestData->class);

   if(dt->nodeType != dtnGrowing && dt->nodeType != dtnLeaf) {
      for(i = 0 ; i < VALLength(dt->children) ; i++) {
         _PrintPruneData(VALIndex(dt->children, i), level + 1);
      }
   }
}

static void _PruneNode(DecisionTreePtr dt) {
   PruneDataPtr data = DecisionTreeGetGrowingData(dt);
   DecisionTreePtr current;
   PruneDataPtr currentData;
   long i;

   /* update parent delta errors */
   current = data->parent;
   while(current != 0) {
      currentData = DecisionTreeGetGrowingData(current);
      /* this seems backwards, but by improving errors below the parent
           looks like a less good place to prune */
      currentData->errorDelta -= data->errorDelta;
      current = currentData->parent;
   }

   /* free stuff below the node */
   if(dt->nodeType != dtnGrowing && dt->nodeType != dtnLeaf) {
      for(i = 0 ; i < VALLength(dt->children) ; i++) {
         current = VALIndex(dt->children, i);
         _FreePruneData(current);
         DecisionTreeFree(current);
      }
   }

   /* update this node */
   DecisionTreeSetTypeLeaf(dt);
   DecisionTreeSetClass(dt, data->class);

   data->errorDelta = data->errors;
}

void VFDTREPrune(DecisionTreePtr dt, VoidAListPtr examples) {
   long i;
   int progress;
   DecisionTreePtr pruneNode;

   // propogate the prune data over the whole tree
   _InitPruneData(dt, 0);

   // pass the prune set through the tree recording errors
   for(i = 0 ; i < VALLength(examples) ; i++) {
      _UpdateErrorCounts(dt, VALIndex(examples, i));
   }

   // collect the error deltas
   _InitErrorDelta(dt);
   //_PrintPruneData(dt, 0);

   progress = 1;
   // while there is an improvement
   while(progress) {
      // find the best candidate
      pruneNode = _FindBestPruneNode(dt);

      // prune the best candidate if appropriate
      if(pruneNode != 0) {
         //printf("prune a node\n");
         _PruneNode(pruneNode);
      } else {
         progress = 0;
      }
   }

   // free the prune data
   _FreePruneData(dt);

}
//SimRoiEnd();
