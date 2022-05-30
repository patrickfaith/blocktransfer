#include <string.h>
#include <btdefs.h>
#include <btlist.h>
#include <btblocks.h>
#include <btgraph.h>
#include <btacmdef.h>
#include <btacmstr.h>
#include <btproto.h>

void SaveWindowsObjects(BTtrans *trans, BTWindow *wndw)
{
  if (wndw == NULL ) return ;
  
  char buffer[132];
  
  sprintf(buffer,"wndw %d %d %d %d\n", wndw->X, wndw->Y, wndw->Width, wndw->Height );
  trans->exPort(trans,buffer);

  return ;
}

void SaveSpatialObjects(BTtrans *trans, BTspace *space)
{
  if (space == NULL) return ;
  char buffer[132];
  
  sprintf(buffer,"space %d %d %d  %d %d %d  %d %d %d\n",
    space->Trans[0], space->Trans[1], space->Trans[2],
    space->Scale[0], space->Scale[1], space->Scale[2],
    space->Rot[0],   space->Rot[1],   space->Rot[2] );

  trans->exPort(trans,buffer);
}

void SaveFacetObjects(BTtrans *trans, BTgraph *graph)
{
  if ( graph == NULL ) return ;
  char buffer[132];
  
  switch (graph->Type() )
  { 
    case BT_LINE_GRAPH:
      {
        sprintf(buffer,"facet %d  %d %d %d  %d %d %d \n", graph->Type(),
                graph->pagePos1[0],graph->pagePos1[1],graph->pagePos1[2],
                graph->pagePos2[0],graph->pagePos2[1],graph->pagePos2[2]    
               );
        trans->exPort(trans,buffer);
      }
      break;
    case BT_SQUARE_GRAPH:
      {
        BTchipGraph *cGraph = (BTchipGraph *) graph; 
        sprintf(buffer,"facet %d  %d %d %d  %d %d %d %d %d %d  %d %d %d \n", graph->Type(),
                cGraph->pagePos1[0],cGraph->pagePos1[1],cGraph->pagePos1[2],
                cGraph->pagePos2[0],cGraph->pagePos2[1],cGraph->pagePos2[2],    
                cGraph->pagePos3[0],cGraph->pagePos3[1],cGraph->pagePos3[2],
                cGraph->pagePos4[0],cGraph->pagePos4[1],cGraph->pagePos4[2]    
               );
        trans->exPort(trans,buffer);
      }
      break;
    case BT_CIRCLE_GRAPH:
      break;
    case BT_MEM_GRAPH:
      break;
    case BT_BMP_GRAPH:
      break;
    case BT_ICON_GRAPH:
      break;    
  }
  return ;   
}
  
void SaveGraphicalObjects(BTtrans *trans, BTblock *block)
{ 
  if ( block->Type() == BT_TYPE_VIEW )
  { 
    BTviewBlock *view = (BTviewBlock *) block; 
    BTWindow *wndw = view->graphWin;    
    SaveWindowsObjects(trans , wndw);
  }
  
  SaveSpatialObjects(trans, block->SpacePtr());
  
  SaveFacetObjects(trans, block->graph);
}

//
//


void LoadWindowsObjects(char *buffer, BTblock *block)
{
  if (block->Type() != BT_TYPE_VIEW ) return ;
  
  char str[30];
  int i[4];
  BTviewBlock *vBlock = (BTviewBlock *) block;
 
  BTWindow *wndw = vBlock->graphWin;
  if ( wndw == NULL ) return ;
  sscanf(buffer,"%s %d %d %d %d",str,&i[0],&i[1],&i[2],&i[3]);
  
  wndw->X = (BTfix) i[0];
  wndw->Y = (BTfix) i[1];
  wndw->Width = (BTfix) i[2];
  wndw->Height =  (BTfix) i[3];

  return ;
}

void LoadSpatialObjects(char *buffer, BTblock *block)
{
  
  BTspace *space = block->SpacePtr() ;
  if (space == NULL) return ;

  char str[30];
  int i[8];
  
  sscanf(buffer,"%s %d %d %d  %d %d %d  %d %d %d",str
                    ,&i[0],&i[1],&i[2],&i[3],&i[4],&i[5],&i[6],&i[7],&i[8]);
                    
  space->Trans[0]	= (BTfix) i[0];
  space->Trans[1]	= (BTfix) i[1];
  space->Trans[2]	= (BTfix) i[2];
  space->Scale[0]	= (BTfix) i[3];
  space->Scale[1]	= (BTfix) i[4];
  space->Scale[2]	= (BTfix) i[5];
  space->Rot[0]		= (BTfix) i[6];
  space->Rot[1]		= (BTfix) i[7];
  space->Rot[2]		= (BTfix) i[8];

}

void LoadFacetObjects(char *buffer, BTblock *block)
{
  
  char str[30];
  int i[13];
  int count;
  
  count = sscanf(buffer,"%s %d  %d %d %d  %d %d %d  %d %d %d  %d %d %d",str
                    ,&i[0],&i[1],&i[2],&i[3],&i[4],&i[5],&i[6],&i[7],&i[8]
                    ,&i[9],&i[10],&i[11],&i[12]);
  if ( count < 4 ) return ;
  
  if ( block->graph != NULL )
    delete block->graph;
    
  switch (i[0] )
  { 
    case BT_LINE_GRAPH:
      { 
        block->graph = new BTgraph();
        block->graph->pagePos1[0] = i[1];
        block->graph->pagePos1[1] = i[2];
        block->graph->pagePos1[2] = i[3];
        block->graph->pagePos2[0] = i[4];
        block->graph->pagePos2[1] = i[5];
        block->graph->pagePos2[2] = i[6];
      }
      break;
    case BT_SQUARE_GRAPH:
      { 
        BTchipGraph *cGraph = new BTchipGraph();
        cGraph->pagePos1[0] = i[1];
        cGraph->pagePos1[1] = i[2];
        cGraph->pagePos1[2] = i[3];
        cGraph->pagePos2[0] = i[4];
        cGraph->pagePos2[1] = i[5];
        cGraph->pagePos2[2] = i[6];
        cGraph->pagePos3[0] = i[7];
        cGraph->pagePos3[1] = i[8];
        cGraph->pagePos3[2] = i[9];
        cGraph->pagePos4[0] = i[10];
        cGraph->pagePos4[1] = i[11];
        cGraph->pagePos4[2] = i[12];
        block->graph = cGraph;
      }
      break;
    case BT_CIRCLE_GRAPH:
      break;
    case BT_MEM_GRAPH:
      break;
    case BT_BMP_GRAPH:
      break;
    case BT_ICON_GRAPH:
      break;    
  }
  return ;   
}
  
void LoadGraphicalObjects(BTtrans *trans, BTblock *block)
{

  short done = 0 ;
  char *buffer;

  while ( done == 0 )
  {
    buffer = trans->inPort(NULL);
    if (strncmpi( buffer,"wndw", 4) == 0 )
    {
      LoadWindowsObjects(buffer, block);
    }
    else if (strncmpi( buffer,"space",5) == 0 )
    {
      LoadSpatialObjects(buffer, block );
    }
    else if (strncmpi( buffer,"facet",5) == 0 )
    {
      LoadFacetObjects(buffer, block );
    }
    else
    { 
      
      trans->exPort(NULL,buffer);
      done = 1;
    }
    delete buffer;   
  }
}


