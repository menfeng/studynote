//1
	printf("==============\n");
	gc = young->gc.gc_next;

	f=fopen("D:\\1.txt","a");
    for (; gc != young; gc=gc->gc.gc_next){
		if(gc->gc.gc_refs!=0){
			PyObject* obj= FROM_GC(gc);
			fprintf(f, obj->ob_type->tp_name);
//			PyObject* ll=PyLong_FromVoidPtr(obj);
			fprintf(f, "%d---%d\n",gc->gc.gc_refs,obj);
//			(obj->ob_type)->tp_print(obj,f,0);
			printf("%d\n",gc->gc.gc_refs);
		}
	}
	fclose(f);
	printf("==================\n");

//2
	PyObject* target=PySys_GetObject("stdout")
	char temp[256]={0};
	sprintf(temp,"%s %s",PyString_AsString(s),"a")
	PyFile_WriteString(temp,target)

