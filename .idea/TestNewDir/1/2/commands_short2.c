/* checks for the existence of the external command file and processes all
   commands found in it */
void check_for_external_commands(void){
	/* process all commands found in the buffer */
	while(1){

		if(!strcmp(command_id,"ENTER_STANDBY_MODE") || !strcmp(command_id,"DISABLE_NOTIFICATIONS"))
			command_type=CMD_DISABLE_NOTIFICATIONS;
		else if(!strcmp(command_id,"ENTER_ACTIVE_MODE") || !strcmp(command_id,"ENABLE_NOTIFICATIONS"))
			command_type=CMD_ENABLE_NOTIFICATIONS;
		else if(!strcmp(command_id,"SCHEDULE_SERVICEGROUP_SVC_DOWNTIME"))
			command_type=CMD_SCHEDULE_SERVICEGROUP_SVC_DOWNTIME;


		/**** UNKNOWN COMMAND ****/
		else{
			/* log the bad external command */
			snprintf(buffer,sizeof(buffer),"Warning: Unrecognized external command -> %s;%s\n",command_id,args);
			buffer[sizeof(buffer)-1]='\x0';
			write_to_all_logs(buffer,NSLOG_EXTERNAL_COMMAND | NSLOG_RUNTIME_WARNING);

			continue;
		        }

	return;
        }

