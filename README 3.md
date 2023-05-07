# CITS3002-Project

project draft:

---------------------- code for reading input from radio button (mcq): -----------------------------

// Parse the HTTP request to extract the POST data
  char* post_data = NULL;   // should be char post_data[1024]
  char* body_start = strstr(buffer, "\r\n\r\n");    // should be char body_start[1024]
  if (body_start != NULL) {
    post_data = body_start + 4;
  }

  // Extract the value of the "choice" parameter from the POST data
  char* choice_value = NULL;    // should be char choice_value[1024]
  if (post_data != NULL) {
    char* choice_start = strstr(post_data, "mcq="); // should be char choice_start[1024]
    if (choice_start != NULL) {
      choice_start += strlen("mcq=");
      char* choice_end = strchr(choice_start, '&'); // should be char choice_end[1024]
      if (choice_end == NULL) {
        choice_end = post_data + strlen(post_data);
      }
      choice_value = strndup(choice_start, choice_end - choice_start);
    }
  }

  // Generate a response based on the POST data
  if (choice_value == NULL) {
    response_body = strdup("No radio button choice found\n");
  } else {
    char response_template[] = "Received radio button choice: %s\n";
    int response_body_size = snprintf(NULL, 0, response_template, choice_value) + 1;
    response_body = malloc(response_body_size);
    snprintf(response_body, response_body_size, response_template, choice_value);
  }
  
  // String to send to QB for grading: choice_value
  
----------------------------------------------- end of code ---------------------------------------------------------------

------------------------- code for reading http post request from textarea input (pcq) ------------------------------------

// Extract POST data from HTTP request
    char *post_data = strstr(buffer, "\r\n\r\n");
    if (post_data != NULL) {
        post_data += 4; // skip past the "\r\n\r\n"
        printf("POST data: %s\n", post_data);
        // parse the POST data to retrieve the textarea input
        // process the data as needed
    }
    
    // Return response to client
    send(new_socket, response, strlen(response), 0);
    printf("Response sent\n");
    
------------------------------------------------- end of code --------------------------------------------------------
