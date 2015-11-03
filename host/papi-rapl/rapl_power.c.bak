
#include <stdio.h>
#include <stdlib.h>
#include "papi_test.h"

#define MAX_RAPL_EVENTS 64

static int retval,cid,rapl_cid=-1,numcmp;
static int EventSet = PAPI_NULL;
static long long *values;
static int num_events=0;
static int code;
static char event_names[MAX_RAPL_EVENTS][PAPI_MAX_STR_LEN];
static char units[MAX_RAPL_EVENTS][PAPI_MIN_STR_LEN];
static int data_type[MAX_RAPL_EVENTS];
static int r,i, do_wrap = 0;
static const PAPI_component_info_t *cmpinfo = NULL;
static PAPI_event_info_t evinfo;
static long long before_time,after_time;
static double elapsed_time;

void rapl_power_start()
{
	/* Set TESTS_QUIET variable */
//	tests_quiet( argc, argv );
//	if ( argc > 1 )
//		if ( strstr( argv[1], "-w" ) )
//			do_wrap = 1;

	/* PAPI Initialization */
	retval = PAPI_library_init( PAPI_VER_CURRENT );
	if ( retval != PAPI_VER_CURRENT ) {
		test_fail(__FILE__, __LINE__,"PAPI_library_init failed\n",retval);
	}

	if (!TESTS_QUIET) {
		printf("Trying all RAPL events\n");
	}

	numcmp = PAPI_num_components();

	for(cid=0; cid<numcmp; cid++) {

		if ( (cmpinfo = PAPI_get_component_info(cid)) == NULL) {
			test_fail(__FILE__, __LINE__,"PAPI_get_component_info failed\n", 0);
		}

		if (strstr(cmpinfo->name,"rapl")) {

			rapl_cid=cid;

			if (!TESTS_QUIET) {
				printf("Found rapl component at cid %d\n",rapl_cid);
			}

			if (cmpinfo->disabled) {
				if (!TESTS_QUIET) {
					printf("RAPL component disabled: %s\n",
						   cmpinfo->disabled_reason);
				} 
				test_skip(__FILE__,__LINE__,"RAPL component disabled",0);
			}
			break;
		}
	}

	/* Component not found */
	if (cid==numcmp) {
		test_skip(__FILE__,__LINE__,"No rapl component found\n",0);
	}

	/* Create EventSet */
	retval = PAPI_create_eventset( &EventSet );
	if (retval != PAPI_OK) {
		test_fail(__FILE__, __LINE__, 
				  "PAPI_create_eventset()",retval);
	}

	/* Add all events */

	code = PAPI_NATIVE_MASK;

	r = PAPI_enum_cmp_event( &code, PAPI_ENUM_FIRST, rapl_cid );

	while ( r == PAPI_OK ) {

		retval = PAPI_event_code_to_name( code, event_names[num_events] );
		if ( retval != PAPI_OK ) {
			printf("Error translating %#x\n",code);
			test_fail( __FILE__, __LINE__, 
					   "PAPI_event_code_to_name", retval );
		}

		retval = PAPI_get_event_info(code,&evinfo);
		if (retval != PAPI_OK) {
			test_fail( __FILE__, __LINE__,
					   "Error getting event info\n",retval);
		}

		strncpy(units[num_events],evinfo.units,sizeof(units[0])-1);
		// buffer must be null terminated to safely use strstr operation on it below
		units[num_events][sizeof(units[0])-1] = '\0';

		data_type[num_events] = evinfo.data_type;

		retval = PAPI_add_event( EventSet, code );
		if (retval != PAPI_OK) {
			break; /* We've hit an event limit */
		}
		num_events++;

		r = PAPI_enum_cmp_event( &code, PAPI_ENUM_EVENTS, rapl_cid );
	}

	values = (long long *)calloc(num_events,sizeof(long long));
	if (values==NULL) {
		test_fail(__FILE__, __LINE__, 
				  "No memory",retval);
	}

	if (!TESTS_QUIET) {
		printf("\nStarting measurements...\n\n");
	}

	/* Start Counting */
	before_time=PAPI_get_real_nsec();
	retval = PAPI_start( EventSet);
	if (retval != PAPI_OK) {
		test_fail(__FILE__, __LINE__, "PAPI_start()",retval);
	}

}

void rapl_power_stop()
{
	
	/* Stop Counting */
	after_time=PAPI_get_real_nsec();
	retval = PAPI_stop( EventSet, values);
	if (retval != PAPI_OK) {
		test_fail(__FILE__, __LINE__, "PAPI_stop()",retval);
	}

	elapsed_time=((double)(after_time-before_time))/1.0e9;

	if (!TESTS_QUIET) {
		printf("\nStopping measurements, took %.3fs, gathering results...\n\n",
			   elapsed_time);

		printf("Scaled energy measurements:\n");

		for(i=0;i<num_events;i++) {
			if (strstr(units[i],"nJ")) {

				printf("%-40s%12.6f J\t(Average Power %.1fW)\n",
					   event_names[i],
					   (double)values[i]/1.0e9,
					   ((double)values[i]/1.0e9)/elapsed_time);
			}
		}

		printf("\n");
		printf("Energy measurement counts:\n");

		for(i=0;i<num_events;i++) {
			if (strstr(event_names[i],"ENERGY_CNT")) {
				printf("%-40s%12lld\t%#08llx\n", event_names[i], values[i], values[i]);
			}
		}

		printf("\n");
		printf("Scaled Fixed values:\n");

		for(i=0;i<num_events;i++) {
			if (!strstr(event_names[i],"ENERGY")) {
				if (data_type[i] == PAPI_DATATYPE_FP64) {

					union {
						long long ll;
						double fp;
					} result;

					result.ll=values[i];
					printf("%-40s%12.3f %s\n", event_names[i], result.fp, units[i]);
				}
			}
		}

		printf("\n");
		printf("Fixed value counts:\n");

		for(i=0;i<num_events;i++) {
			if (!strstr(event_names[i],"ENERGY")) {
				if (data_type[i] == PAPI_DATATYPE_UINT64) {
					printf("%-40s%12lld\t%#08llx\n", event_names[i], values[i], values[i]);
				}
			}
		}

	}

	/* Done, clean up */
	retval = PAPI_cleanup_eventset( EventSet );
	if (retval != PAPI_OK) {
		test_fail(__FILE__, __LINE__, 
				  "PAPI_cleanup_eventset()",retval);
	}

	retval = PAPI_destroy_eventset( &EventSet );
	if (retval != PAPI_OK) {
		test_fail(__FILE__, __LINE__, 
				  "PAPI_destroy_eventset()",retval);
	}

	test_pass( __FILE__, NULL, 0 );
}

