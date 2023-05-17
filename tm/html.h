// Student 1: Allison Lau   (23123849)
// Student 2: Alicia Lau    (22955092)
// Student 3: Li-Anne Long  (23192171)

// HTML variables
char *backButton   = "<form method='post'><input type='submit' name='back' value='Back'></form>";
char *nextButton   = "<form method='post'><input type='submit' name='next' value='Next'></form>";
char *logoutButton = "<form method='post'><input type='submit' name='logout' value='Logout'></form>";
char *seeButton = "<form method='post'><input type='submit' name='see' value='See'></form>";
char *failMessage  = "<p>Login failed. Try again.</p><br>";
char *wrongAnswer  = "<p>Your answer is wrong. Try again.</p>";
char *correctMessage = "<p>Your answer is correct!</p>"; 
char *wrongMessage   = "<p>Your answer is wrong!</p>";
char *blueBG = "style='background-color:#BBECF6'";
char *correctColor = "style='background-color:#C6F099'";
char *wrongColor = "style='background-color:#F6B0AD'";
char *idleColor = "style='background-color:#F0F29F'";
// Allow tabbing got HTML textarea
// Reference - https://stackoverflow.com/questions/6637341/use-tab-to-indent-in-textarea
char *onKeyDown = "onkeydown=\"if(event.keyCode===9){var v=this.value,s=this.selectionStart,e=this.selectionEnd;this.value=v.substring(0, s)+'\t'+v.substring(e);this.selectionStart=this.selectionEnd=s+1;return false;}\"";