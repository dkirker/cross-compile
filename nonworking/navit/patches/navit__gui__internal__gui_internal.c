*** build/src/navit/gui/internal/gui_internal.c	2010-03-17 22:51:42.000000000 +0100
--- gui_internal.c	2010-03-17 22:51:05.000000000 +0100
***************
*** 5901,5908 ****
  	      this->text_foreground_color=*attr->u.color;
  	else
  	      this->text_foreground_color=COLOR_WHITE;
! 
! 	this->text_background_color=COLOR_BLACK;
  	if( (attr=attr_search(attrs,NULL,attr_columns)))
  	      this->cols=attr->u.num;
  	if( (attr=attr_search(attrs,NULL,attr_osd_configuration)))
--- 5901,5908 ----
  	      this->text_foreground_color=*attr->u.color;
  	else
  	      this->text_foreground_color=COLOR_WHITE;
! 	// SCIP
! 	this->text_background_color=COLOR_WHITE;
  	if( (attr=attr_search(attrs,NULL,attr_columns)))
  	      this->cols=attr->u.num;
  	if( (attr=attr_search(attrs,NULL,attr_osd_configuration)))
