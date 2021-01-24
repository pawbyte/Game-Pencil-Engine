/*

 MIT License

 Copyright © 2020 Samuel Venable

 Permission is hereby granted, free of charge, to any person obtaining a copy
 of this software and associated documentation files (the "Software"), to deal
 in the Software without restriction, including without limitation the rights
 to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 copies of the Software, and to permit persons to whom the Software is
 furnished to do so, subject to the following conditions:

 The above copyright notice and this permission notice shall be included in all
 copies or substantial portions of the Software.

 THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 SOFTWARE.

*/

#import "../../dlgmodule.h"
#import "config.h"
#import <climits>
#import <cstdlib>
#import <cstring>
#import <cstdio>
#import <fstream>
#import <string>

#import <sys/stat.h>
#import <Cocoa/Cocoa.h>
#import <libproc.h>
#import <unistd.h>

enum BUTTON_TYPES {
  BUTTON_ABORT,
  BUTTON_IGNORE,
  BUTTON_OK,
  BUTTON_CANCEL,
  BUTTON_YES,
  BUTTON_NO,
  BUTTON_RETRY
};

using std::string;

const char *cocoa_widget_get_button_name(int type) {
  return widget_get_button_name(type);
}

  namespace {

    string caption;
    string current_icon;

    int const btn_array_len = 7;
    string btn_array[btn_array_len] = { "Abort", "Ignore", "OK", "Cancel", "Yes", "No", "Retry" };

char *owner = NULL;

const char *escquotes(const char *str) {
  NSString *nsstr = [NSString stringWithUTF8String:str];
  return [[nsstr stringByReplacingOccurrencesOfString:@"\"" withString:@"\\\""] UTF8String];
}

const char *cocoa_get_bundle_id() {
  char buffer[PROC_PIDPATHINFO_MAXSIZE];
  if (proc_pidpath(getpid(), buffer, sizeof(buffer)) > 0) {
    NSString *result = [[NSString stringWithUTF8String:buffer] stringByAppendingString:@"\0"];
    result = [result stringByDeletingLastPathComponent];
    result = [result stringByDeletingLastPathComponent];
    result = [result stringByDeletingLastPathComponent];
    return [[[NSBundle bundleWithPath:result] bundleIdentifier] UTF8String];
  }
  return "";
}

int cstring_to_integer(const char *cstr) {
  return (int)strtol(cstr, NULL, 10);
}

const char *integer_to_cstring(int numb) {
  return [[@(numb) stringValue] UTF8String];
}

const char *cstring_concat(const char *cstr1, const char *cstr2) {
  return [[[NSString stringWithUTF8String:cstr1]
  stringByAppendingString:[NSString stringWithUTF8String:cstr2]] UTF8String];
}

bool file_exists(const char *fname) {
  NSFileManager *fileManager = [NSFileManager defaultManager];
  return [fileManager fileExistsAtPath:[NSString stringWithUTF8String:fname]];
}

const char *evaluate_shell(const char *command) {
  char *buffer = NULL;
  size_t buffer_size = 0;
  NSString *result = @"";

  FILE *file = popen(command, "r");
  while (getline(&buffer, &buffer_size, file) != -1)
    result = [result stringByAppendingString:[NSString stringWithUTF8String:buffer]];

  free(buffer);
  pclose(file);

  if ([result hasSuffix:@"\n"])
    result = [result substringToIndex:[result length] - 1];
  
  return [result UTF8String];
}

char *cocoa_widget_get_owner() {
  return owner;
}

void cocoa_widget_set_owner(char *hwnd) {
  owner = hwnd;
}

int cocoa_show_message(const char *str, bool has_cancel, const char *icon, const char *title) {
  if (![NSThread isMainThread]) {
    evaluate_shell(cstring_concat(cstring_concat("chmod 777 \"", escquotes([[[NSBundle mainBundle] resourcePath] UTF8String])), "/dlgmod\""));
    const char *defaultIcon = [[[[NSBundle mainBundle] infoDictionary] valueForKey:@"CFBundleIconFile"] UTF8String];
    const char *currentIcon = (file_exists(icon)) ? icon : defaultIcon;
    return cstring_to_integer(evaluate_shell(cstring_concat(cstring_concat(cstring_concat(cstring_concat(
    cstring_concat(cstring_concat(cstring_concat(cstring_concat(cstring_concat("\"", [[[NSBundle mainBundle] resourcePath]
    UTF8String]), "/dlgmod\" --"), (has_cancel) ? "show-message-cancelable \"" :
    "show-message \""), escquotes(str)), "\" \""), escquotes(currentIcon)), "\" \""), escquotes(title)), "\"")));
  }

  NSString *myTitle = [NSString stringWithUTF8String:title];
  NSString *myStr = [NSString stringWithUTF8String:str];

  NSAlert *alert = [[NSAlert alloc] init];
  NSImage *image = [[NSImage alloc] initWithContentsOfFile:[NSString stringWithUTF8String:icon]];
  [alert setIcon:image];
  [alert setMessageText:myTitle];
  [alert setInformativeText:myStr];
  [alert addButtonWithTitle:[NSString stringWithUTF8String:cocoa_widget_get_button_name(BUTTON_OK)]];
  if (has_cancel) [alert addButtonWithTitle:[NSString stringWithUTF8String:cocoa_widget_get_button_name(BUTTON_CANCEL)]];
  [alert setAlertStyle:(NSAlertStyle)1];

  NSModalResponse responseTag = [alert runModal];
  [image release];
  [alert release];

  if (responseTag == NSAlertFirstButtonReturn)
    return 1;

  return -1;
}

int cocoa_show_question(const char *str, bool has_cancel, const char *icon, const char *title) {
  if (![NSThread isMainThread]) {
    evaluate_shell(cstring_concat(cstring_concat("chmod 777 \"", escquotes([[[NSBundle mainBundle] resourcePath] UTF8String])), "/dlgmod\""));
    const char *defaultIcon = [[[[NSBundle mainBundle] infoDictionary] valueForKey:@"CFBundleIconFile"] UTF8String];
    const char *currentIcon = (file_exists(icon)) ? icon : defaultIcon;
    return cstring_to_integer(evaluate_shell(cstring_concat(cstring_concat(cstring_concat(cstring_concat(
    cstring_concat(cstring_concat(cstring_concat(cstring_concat(cstring_concat("\"", [[[NSBundle mainBundle] resourcePath]
    UTF8String]), "/dlgmod\" --"), (has_cancel) ? "show-question-cancelable \"" :
    "show-question \""), escquotes(str)), "\" \""), escquotes(currentIcon)), "\" \""), escquotes(title)), "\"")));
  }

  NSString *myTitle = [NSString stringWithUTF8String:title];
  NSString *myStr = [NSString stringWithUTF8String:str];

  NSAlert *alert = [[NSAlert alloc] init];
  NSImage *image = [[NSImage alloc] initWithContentsOfFile:[NSString stringWithUTF8String:icon]];
  [alert setIcon:image];
  [alert setMessageText:myTitle];
  [alert setInformativeText:myStr];
  [alert addButtonWithTitle:[NSString stringWithUTF8String:cocoa_widget_get_button_name(BUTTON_YES)]];
  [alert addButtonWithTitle:[NSString stringWithUTF8String:cocoa_widget_get_button_name(BUTTON_NO)]];
  if (has_cancel) [alert addButtonWithTitle:[NSString stringWithUTF8String:cocoa_widget_get_button_name(BUTTON_CANCEL)]];
  [alert setAlertStyle:(NSAlertStyle)1];

  NSModalResponse responseTag = [alert runModal];
  [image release];
  [alert release];

  if (responseTag == NSAlertFirstButtonReturn)
    return 1;

  if (responseTag == NSAlertSecondButtonReturn)
    return 0;

  return -1;
}

int cocoa_show_attempt(const char *str, const char *icon, const char *title) {
  if (![NSThread isMainThread]) {
    evaluate_shell(cstring_concat(cstring_concat("chmod 777 \"", escquotes([[[NSBundle mainBundle] resourcePath] UTF8String])), "/dlgmod\""));
    const char *defaultIcon = [[[[NSBundle mainBundle] infoDictionary] valueForKey:@"CFBundleIconFile"] UTF8String];
    const char *currentIcon = (file_exists(icon)) ? icon : defaultIcon;
    return cstring_to_integer(evaluate_shell(cstring_concat(cstring_concat(cstring_concat(cstring_concat(
    cstring_concat(cstring_concat(cstring_concat(cstring_concat(cstring_concat("\"", [[[NSBundle mainBundle] resourcePath]
    UTF8String]), "/dlgmod\" --"), "show-attempt \""), escquotes(str)), "\" \""), escquotes(currentIcon)), "\" \""), escquotes(title)), "\"")));
  }

  NSString *myStr = [NSString stringWithUTF8String:str];
  NSAlert *alert = [[NSAlert alloc] init];
  NSImage *image = [[NSImage alloc] initWithContentsOfFile:[NSString stringWithUTF8String:icon]];
  [alert setIcon:image];
  [alert setMessageText:@"Error"];

  if (![[NSString stringWithUTF8String:title] isEqualToString:@""])
    [alert setMessageText:[NSString stringWithUTF8String:title]];

  [alert setInformativeText:myStr];
  [alert addButtonWithTitle:[NSString stringWithUTF8String:cocoa_widget_get_button_name(BUTTON_RETRY)]];
  [alert addButtonWithTitle:[NSString stringWithUTF8String:cocoa_widget_get_button_name(BUTTON_CANCEL)]];
  [alert setAlertStyle:(NSAlertStyle)2];

  NSModalResponse responseTag = [alert runModal];
  [image release];
  [alert release];

  if (responseTag == NSAlertFirstButtonReturn)
    return 0;

  return -1;
}

int cocoa_show_error(const char *str, bool _abort, const char *icon, const char *title) {
  if (![NSThread isMainThread]) {
    evaluate_shell(cstring_concat(cstring_concat("chmod 777 \"", escquotes([[[NSBundle mainBundle] resourcePath] UTF8String])), "/dlgmod\""));
    const char *defaultIcon = [[[[NSBundle mainBundle] infoDictionary] valueForKey:@"CFBundleIconFile"] UTF8String];
    const char *currentIcon = (file_exists(icon)) ? icon : defaultIcon;
    return cstring_to_integer(evaluate_shell(cstring_concat(cstring_concat(cstring_concat(cstring_concat(
    cstring_concat(cstring_concat(cstring_concat(cstring_concat(cstring_concat("\"", [[[NSBundle mainBundle] resourcePath]
    UTF8String]), "/dlgmod\" --"), "show-error \""), escquotes(str)), (_abort) ? "\" 1 \"" : "\" 0 \""),
    escquotes(currentIcon)), "\" \""), escquotes(title)), "\"")));
  }

  NSString *myStr = [NSString stringWithUTF8String:str];
  NSAlert *alert = [[NSAlert alloc] init];
  NSImage *image = [[NSImage alloc] initWithContentsOfFile:[NSString stringWithUTF8String:icon]];
  [alert setIcon:image];
  [alert setMessageText:@"Error"];

  if (![[NSString stringWithUTF8String:title] isEqualToString:@""])
    [alert setMessageText:[NSString stringWithUTF8String:title]];

  [alert setInformativeText:myStr];
  [alert addButtonWithTitle:[NSString stringWithUTF8String:cocoa_widget_get_button_name(BUTTON_ABORT)]];
  if (!_abort) [alert addButtonWithTitle:[NSString stringWithUTF8String:cocoa_widget_get_button_name(BUTTON_IGNORE)]];
  [alert setAlertStyle:(NSAlertStyle)2];

  NSModalResponse responseTag = [alert runModal];
  [image release];
  [alert release];

  if (responseTag == NSAlertFirstButtonReturn || _abort)
    return 1;

  return -1;
}

const char *cocoa_input_box(const char *str, const char *def, const char *icon, const char *title, bool numbers) {
  if (![NSThread isMainThread]) {
    evaluate_shell(cstring_concat(cstring_concat("chmod 777 \"", escquotes([[[NSBundle mainBundle] resourcePath] UTF8String])), "/dlgmod\""));
    const char *defaultIcon = [[[[NSBundle mainBundle] infoDictionary] valueForKey:@"CFBundleIconFile"] UTF8String];
    const char *currentIcon = (file_exists(icon)) ? icon : defaultIcon;
    return evaluate_shell(cstring_concat(cstring_concat(cstring_concat(cstring_concat(
    cstring_concat(cstring_concat(cstring_concat(cstring_concat(cstring_concat(cstring_concat(cstring_concat("\"", [[[NSBundle mainBundle] resourcePath]
    UTF8String]), "/dlgmod\" --"), (numbers) ? "get-integer \"" : "get-string \""), escquotes(str)), "\" \""), escquotes(def)), "\" \""),
    escquotes(currentIcon)), "\" \""), escquotes(title)), "\""));
  }

  NSString *myTitle = [NSString stringWithUTF8String:title];
  NSString *myStr = [NSString stringWithUTF8String:str];
  NSString *myDef = [NSString stringWithUTF8String:def];

  NSAlert *alert = [[NSAlert alloc] init];
  NSImage *image = [[NSImage alloc] initWithContentsOfFile:[NSString stringWithUTF8String:icon]];
  [alert setIcon:image];
  [alert setMessageText:myTitle];
  [alert setInformativeText:myStr];
  [alert addButtonWithTitle:[NSString stringWithUTF8String:cocoa_widget_get_button_name(BUTTON_OK)]];
  [alert addButtonWithTitle:[NSString stringWithUTF8String:cocoa_widget_get_button_name(BUTTON_CANCEL)]];

  NSTextField *input = [[NSTextField alloc] initWithFrame:NSMakeRect(0, 0, 300, 22)];
  [[input cell] setWraps:NO];
  [[input cell] setScrollable:YES];
  [input setMaximumNumberOfLines:1];
  [input setStringValue:myDef];
  [alert setAccessoryView:input];
  NSView *myAccessoryView = [alert accessoryView];
  [[alert window] setInitialFirstResponder:myAccessoryView];

  NSModalResponse responseTag = [alert runModal];
  const char *result;

  if (responseTag == NSAlertFirstButtonReturn) {
    [input validateEditing];
    result = [[input stringValue] UTF8String];
  } else
    result = "";

  [input release];
  [image release];
  [alert release];

  return result;
}

const char *cocoa_password_box(const char *str, const char *def, const char *icon, const char *title, bool numbers) {
  if (![NSThread isMainThread]) {
    evaluate_shell(cstring_concat(cstring_concat("chmod 777 \"", escquotes([[[NSBundle mainBundle] resourcePath] UTF8String])), "/dlgmod\""));
    const char *defaultIcon = [[[[NSBundle mainBundle] infoDictionary] valueForKey:@"CFBundleIconFile"] UTF8String];
    const char *currentIcon = (file_exists(icon)) ? icon : defaultIcon;
    return evaluate_shell(cstring_concat(cstring_concat(cstring_concat(cstring_concat(
    cstring_concat(cstring_concat(cstring_concat(cstring_concat(cstring_concat(cstring_concat(cstring_concat("\"", [[[NSBundle mainBundle] resourcePath]
    UTF8String]), "/dlgmod\" --"), (numbers) ? "get-passcode \"" : "get-password \""), escquotes(str)), "\" \""), escquotes(def)), "\" \""),
    escquotes(currentIcon)), "\" \""), escquotes(title)), "\""));
  }

  NSString *myTitle = [NSString stringWithUTF8String:title];
  NSString *myStr = [NSString stringWithUTF8String:str];
  NSString *myDef = [NSString stringWithUTF8String:def];

  NSAlert *alert = [[NSAlert alloc] init];
  NSImage *image = [[NSImage alloc] initWithContentsOfFile:[NSString stringWithUTF8String:icon]];
  [alert setIcon:image];
  [alert setMessageText:myTitle];
  [alert setInformativeText:myStr];
  [alert addButtonWithTitle:[NSString stringWithUTF8String:cocoa_widget_get_button_name(BUTTON_OK)]];
  [alert addButtonWithTitle:[NSString stringWithUTF8String:cocoa_widget_get_button_name(BUTTON_CANCEL)]];

  NSSecureTextField *input = [[NSSecureTextField alloc] initWithFrame:NSMakeRect(0, 0, 300, 22)];
  [[input cell] setWraps:NO];
  [[input cell] setScrollable:YES];
  [input setMaximumNumberOfLines:1];
  [input setStringValue:myDef];
  [alert setAccessoryView:input];
  NSView *myAccessoryView = [alert accessoryView];
  [[alert window] setInitialFirstResponder:myAccessoryView];

  NSModalResponse responseTag = [alert runModal];
  const char *result;

  if (responseTag == NSAlertFirstButtonReturn) {
    [input validateEditing];
    result = [[input stringValue] UTF8String];
  } else
    result = "";

  [input release];
  [image release];
  [alert release];

  return result;
}

const char *cocoa_get_open_filename(const char *filter, const char *fname, const char *dir, const char *title, bool mselect) {
  if (![NSThread isMainThread]) {
    evaluate_shell(cstring_concat(cstring_concat("chmod 777 \"", escquotes([[[NSBundle mainBundle] resourcePath] UTF8String])), "/dlgmod\""));
    return evaluate_shell(cstring_concat(cstring_concat(cstring_concat(cstring_concat(
    cstring_concat(cstring_concat(cstring_concat(cstring_concat(cstring_concat(cstring_concat(cstring_concat("\"", [[[NSBundle mainBundle] resourcePath]
    UTF8String]), "/dlgmod\" --"), (mselect) ? "get-open-filenames-ext \"" : "get-open-filename-ext \""), escquotes(filter)),
    "\" \""), escquotes(fname)), "\" \""), escquotes(dir)), "\" \""), escquotes(title)), "\""));
  }

  NSOpenPanel *oFilePanel = [NSOpenPanel openPanel];
  [oFilePanel setMessage:[NSString stringWithUTF8String:title]];
  [oFilePanel setDirectoryURL:[NSURL fileURLWithPath:[NSString stringWithUTF8String:dir]]];
  [oFilePanel setCanChooseFiles:YES];
  [oFilePanel setCanChooseDirectories:NO];
  [oFilePanel setCanCreateDirectories:NO];
  [oFilePanel setResolvesAliases:YES];

  if (mselect)
    [oFilePanel setAllowsMultipleSelection:YES];
  else
    [oFilePanel setAllowsMultipleSelection:NO];

  NSView *openView = [[NSView alloc] initWithFrame:NSMakeRect(0, 0, 431, 21)];
  NSPopUpButton *openPop = [[NSPopUpButton alloc] initWithFrame:NSMakeRect(104, 0, 255, 22)];
  NSString *openFilter = [NSString stringWithUTF8String:filter];
  bool openShowAccessory = true;
  bool openAllowAllFiles = false;

  if ([openFilter length] == 0 ||
    ([openFilter rangeOfString:@"|"].location != NSNotFound &&
     [openFilter rangeOfString:@"|"].location == 0))
    openShowAccessory = false;

  if ([openFilter length] == 0 ||
    [openFilter rangeOfString:@"|"].location == NSNotFound) {
    openFilter = [openFilter stringByAppendingString:@"|"];
    openAllowAllFiles = true;
  }

  int openIndex = 0;
  int openCount = 0, openLength = [openFilter length];
  NSRange openRange = NSMakeRange(0, openLength);

  while (openRange.location != NSNotFound) {
    openRange = [openFilter rangeOfString: @"|" options:0 range:openRange];

    if (openRange.location != NSNotFound) {
      openRange = NSMakeRange(openRange.location + openRange.length, openLength - (openRange.location + openRange.length));
      openCount += 1;
    }
  }

  NSString *openPattern = openFilter;
  openPattern = [openPattern stringByReplacingOccurrencesOfString:@"*." withString:@""];
  openPattern = [openPattern stringByReplacingOccurrencesOfString:@" " withString:@""];

  NSArray *openArray1 = [openFilter componentsSeparatedByString:@"|"];
  NSArray *openArray2 = [openPattern componentsSeparatedByString:@"|"];
  NSMutableArray *openPatternArray = [[NSMutableArray alloc] init];
  NSMutableArray *openDescrArray = [[NSMutableArray alloc] init];

  for (openIndex = 0; openIndex <= openCount; openIndex += 1) {
    if (openIndex % 2) {
      [openPatternArray addObject:[openArray2 objectAtIndex:openIndex]];
    } else {
      [openDescrArray addObject:[openArray1 objectAtIndex:openIndex]];
    }
  }

  NSString *selectedOpenPattern = [openPatternArray objectAtIndex:0];
  NSArray *openPatternItems = [selectedOpenPattern componentsSeparatedByString:@";"];
  [oFilePanel setAllowedFileTypes:openPatternItems];

  if ([openPatternItems containsObject:@"*"] || openAllowAllFiles || !openShowAccessory)
    [oFilePanel setAllowedFileTypes:nil];

  [openPop addItemsWithTitles:openDescrArray];
  [openPop selectItemWithTitle:[openDescrArray objectAtIndex:0]];
  [openView addSubview:openPop];
  [oFilePanel setAccessoryView:openView];

  if (!openShowAccessory)
    [oFilePanel setAccessoryView:nil];

  const char *theOpenResult = "";
  bool initOpenAccessory = false;
  int openPopIndex = 0;

  NSModalSession openSession = [NSApp beginModalSessionForWindow:oFilePanel];
  [oFilePanel center];

  for (;;) {
    if ([NSApp runModalSession:openSession] == NSModalResponseOK) {
      NSURL *theOpenURL;
      NSString *theOpenFile;
      int openURLSize = [[oFilePanel URLs] count];

      if (openURLSize > 1) {
        NSMutableArray *openFileArray = [[NSMutableArray alloc] init];

        for (int openURLIndex = 0; openURLIndex < openURLSize; openURLIndex += 1) {
          [openFileArray addObject:[[[oFilePanel URLs] objectAtIndex:openURLIndex] path]];
        }

        theOpenFile = [openFileArray componentsJoinedByString:@"\n"];
        [openFileArray release];
      } else
        theOpenFile = [[[oFilePanel URLs] objectAtIndex:0] path];

      theOpenResult = [theOpenFile UTF8String];
      break;
    }

    if ([NSApp runModalSession:openSession] == NSModalResponseCancel)
      break;

    if (![oFilePanel isAccessoryViewDisclosed] && !initOpenAccessory) {
      [oFilePanel setAccessoryViewDisclosed:YES];
      initOpenAccessory = true;
    }

    if (openShowAccessory) {
      if ([openPop indexOfSelectedItem] != openPopIndex) {
        selectedOpenPattern = [openPatternArray objectAtIndex:[openPop indexOfSelectedItem]];
        openPatternItems = [selectedOpenPattern componentsSeparatedByString:@";"];

        if ([openPatternItems containsObject:@"*"])
          [oFilePanel setAllowedFileTypes:nil];
        else
          [oFilePanel setAllowedFileTypes:openPatternItems];

        openPopIndex = [openPop indexOfSelectedItem];
      }
    }
  }

  [NSApp endModalSession:openSession];
  [oFilePanel close];

  [openPatternArray release];
  [openDescrArray release];
  [openPop release];
  [openView release];

  return theOpenResult;
}

const char *cocoa_get_save_filename(const char *filter, const char *fname, const char *dir, const char *title) {
  if (![NSThread isMainThread]) {
    evaluate_shell(cstring_concat(cstring_concat("chmod 777 \"", escquotes([[[NSBundle mainBundle] resourcePath] UTF8String])), "/dlgmod\""));
    return evaluate_shell(cstring_concat(cstring_concat(cstring_concat(cstring_concat(
    cstring_concat(cstring_concat(cstring_concat(cstring_concat(cstring_concat(cstring_concat(cstring_concat("\"", [[[NSBundle mainBundle] resourcePath]
    UTF8String]), "/dlgmod\" --"), "get-save-filename-ext \""), escquotes(filter)), "\" \""), escquotes(fname)), "\" \""), escquotes(dir)),
    "\" \""), escquotes(title)), "\""));
  }

  NSSavePanel *sFilePanel = [NSSavePanel savePanel];
  [sFilePanel setMessage:[NSString stringWithUTF8String:title]];
  [sFilePanel setDirectoryURL:[NSURL fileURLWithPath:[NSString stringWithUTF8String:dir]]];
  [sFilePanel setNameFieldStringValue:[[[NSString stringWithUTF8String:fname] lastPathComponent] stringByDeletingPathExtension]];
  [sFilePanel setCanCreateDirectories:YES];

  NSView *saveView = [[NSView alloc] initWithFrame:NSMakeRect(0, 0, 433, 21)];
  NSPopUpButton *savePop = [[NSPopUpButton alloc] initWithFrame:NSMakeRect(105, 0, 255, 22)];
  NSString *saveFilter = [NSString stringWithUTF8String:filter];
  bool saveShowAccessory = true;
  bool saveAllowAllFiles = false;

  if ([saveFilter length] == 0 ||
    ([saveFilter rangeOfString:@"|"].location != NSNotFound &&
     [saveFilter rangeOfString:@"|"].location == 0))
    saveShowAccessory = false;

  if ([saveFilter length] == 0 ||
    [saveFilter rangeOfString:@"|"].location == NSNotFound) {
    saveFilter = [saveFilter stringByAppendingString:@"|"];
    saveAllowAllFiles = true;
  }

  int saveIndex = 0;
  int saveCount = 0, saveLength = [saveFilter length];
  NSRange saveRange = NSMakeRange(0, saveLength);

  while (saveRange.location != NSNotFound) {
    saveRange = [saveFilter rangeOfString: @"|" options:0 range:saveRange];

    if (saveRange.location != NSNotFound) {
      saveRange = NSMakeRange(saveRange.location + saveRange.length, saveLength - (saveRange.location + saveRange.length));
      saveCount += 1;
    }
  }

  NSString *savePattern = saveFilter;
  savePattern = [savePattern stringByReplacingOccurrencesOfString:@"*." withString:@""];
  savePattern = [savePattern stringByReplacingOccurrencesOfString:@" " withString:@""];

  NSArray *saveArray1 = [saveFilter componentsSeparatedByString:@"|"];
  NSArray *saveArray2 = [savePattern componentsSeparatedByString:@"|"];
  NSMutableArray *savePatternArray = [[NSMutableArray alloc] init];
  NSMutableArray *saveDescrArray = [[NSMutableArray alloc] init];

  for (saveIndex = 0; saveIndex <= saveCount; saveIndex += 1) {
    if (saveIndex % 2) {
      [savePatternArray addObject:[saveArray2 objectAtIndex:saveIndex]];
    } else {
      [saveDescrArray addObject:[saveArray1 objectAtIndex:saveIndex]];
    }
  }

  NSString *selectedSavePattern = [savePatternArray objectAtIndex:0];
  NSArray *savePatternItems = [selectedSavePattern componentsSeparatedByString:@";"];
  [sFilePanel setAllowedFileTypes:savePatternItems];

  [sFilePanel setAllowedFileTypes:savePatternItems];

  if ([savePatternItems containsObject:@"*"] || saveAllowAllFiles || !saveShowAccessory)
    [sFilePanel setAllowedFileTypes:nil];

  [savePop addItemsWithTitles:saveDescrArray];
  [savePop selectItemWithTitle:[saveDescrArray objectAtIndex:0]];
  [saveView addSubview:savePop];
  [sFilePanel setAccessoryView:saveView];

  if (!saveShowAccessory)
    [sFilePanel setAccessoryView:nil];

  const char *theSaveResult = "";
  int savePopIndex = 0;

  NSModalSession saveSession = [NSApp beginModalSessionForWindow:sFilePanel];
  [sFilePanel center];

  for (;;) {
    if ([NSApp runModalSession:saveSession] == NSModalResponseOK) {
      NSURL *theSaveURL = [sFilePanel URL];
      NSString *theSaveFile = [theSaveURL path];
      theSaveResult = [theSaveFile UTF8String];
      break;
    }

    if ([NSApp runModalSession:saveSession] == NSModalResponseCancel)
      break;

    if (saveShowAccessory) {
      if ([savePop indexOfSelectedItem] != savePopIndex) {
        selectedSavePattern = [savePatternArray objectAtIndex:[savePop indexOfSelectedItem]];
        savePatternItems = [selectedSavePattern componentsSeparatedByString:@";"];

        if ([savePatternItems containsObject:@"*"])
          [sFilePanel setAllowedFileTypes:nil];
        else
          [sFilePanel setAllowedFileTypes:savePatternItems];

        savePopIndex = [savePop indexOfSelectedItem];
      }
    }
  }

  [NSApp endModalSession:saveSession];
  [sFilePanel close];

  [savePatternArray release];
  [saveDescrArray release];
  [savePop release];
  [saveView release];

  return theSaveResult;
}

const char *cocoa_get_directory(const char *capt, const char *root) {
  if (![NSThread isMainThread]) {
    evaluate_shell(cstring_concat(cstring_concat("chmod 777 \"", escquotes([[[NSBundle mainBundle] resourcePath] UTF8String])), "/dlgmod\""));
    return evaluate_shell(cstring_concat(cstring_concat(
    cstring_concat(cstring_concat(cstring_concat(cstring_concat(cstring_concat("\"", [[[NSBundle mainBundle] resourcePath]
    UTF8String]), "/dlgmod\" --"), "get-directory-alt \""), escquotes(capt)), "\" \""), escquotes(root)), "\""));
  }

  NSOpenPanel* dirPanel = [NSOpenPanel openPanel];
  [dirPanel setMessage:[NSString stringWithUTF8String:capt]];
  [dirPanel setDirectoryURL:[NSURL fileURLWithPath:[NSString stringWithUTF8String:root]]];
  [dirPanel setCanChooseFiles:NO];
  [dirPanel setCanChooseDirectories:YES];
  [dirPanel setCanCreateDirectories:YES];
  [dirPanel setAllowsMultipleSelection:NO];
  [dirPanel setResolvesAliases:YES];
  const char *theFolderResult = "";

  NSModalSession dirSession = [NSApp beginModalSessionForWindow:dirPanel];
  [dirPanel center];
    
  for (;;) {
    if ([NSApp runModalSession:dirSession] == NSModalResponseOK) {
      NSURL *theFolderURL = [[dirPanel URLs] objectAtIndex:0];
      NSString *theFolderPath = [theFolderURL path];
      if (![theFolderPath hasSuffix:@"/"])
        theFolderPath = [theFolderPath stringByAppendingString:@"/"];
      theFolderResult = [theFolderPath UTF8String];
      break;
    }

    if ([NSApp runModalSession:dirSession] == NSModalResponseCancel)
      break;
  }
    
  [NSApp endModalSession:dirSession];
  [dirPanel close];
  
  return theFolderResult;
}

int cocoa_get_color(int defcol, const char *title) {
  if (![NSThread isMainThread]) {
    evaluate_shell(cstring_concat(cstring_concat("chmod 777 \"", escquotes([[[NSBundle mainBundle] resourcePath] UTF8String])), "/dlgmod\""));
    return cstring_to_integer(evaluate_shell(cstring_concat(cstring_concat(
    cstring_concat(cstring_concat(cstring_concat(cstring_concat(cstring_concat("\"", [[[NSBundle mainBundle] resourcePath]
    UTF8String]), "/dlgmod\" --"), "get-color-ext \""), integer_to_cstring(defcol)), "\" \""), escquotes(title)), "\"")));
  }

  int redValue = defcol & 0xFF;
  int greenValue = (defcol >> 8) & 0xFF;
  int blueValue = (defcol >> 16) & 0xFF;

  NSInteger buttonWidth = 82, buttonHeight = 30, rightofButtons = 8;
  NSString *myColorTitle = [NSString stringWithUTF8String:title];
  NSColor *myDefCol = [NSColor colorWithCalibratedRed:redValue green:greenValue blue:blueValue alpha:1.0f];
  NSColorPanel *myColorPanel = [NSColorPanel sharedColorPanel];

  [myColorPanel setFrame:NSMakeRect(0, 0, 229, 399) display:YES animate:NO];
  if ([myColorTitle length] > 0) [myColorPanel setTitle:myColorTitle];
  NSSize myColorSize = [[myColorPanel contentView] frame].size;
  NSView *myViewView = [[NSView alloc] initWithFrame:NSMakeRect(0, 0, myColorSize.width, buttonHeight)];
  NSView *myButtonView = [[NSView alloc] initWithFrame:NSMakeRect(0, 0, myColorSize.width, buttonHeight)];
  NSButton *myOKButton = [[NSButton alloc] initWithFrame:NSMakeRect(myColorSize.width - buttonWidth - rightofButtons, 0, buttonWidth, buttonHeight)];
  NSButton *myCancelButton = [[NSButton alloc] initWithFrame:NSMakeRect(myColorSize.width - (buttonWidth * 2) - rightofButtons, 0, buttonWidth, buttonHeight)];

  [myOKButton setTitle:[NSString stringWithUTF8String:cocoa_widget_get_button_name(BUTTON_OK)]];
  [myOKButton setAlternateTitle:[NSString stringWithUTF8String:cocoa_widget_get_button_name(BUTTON_OK)]];
  [myOKButton setBezelStyle:(NSBezelStyle)1];
  [myCancelButton setTitle:[NSString stringWithUTF8String:cocoa_widget_get_button_name(BUTTON_CANCEL)]];
  [myCancelButton setAlternateTitle:[NSString stringWithUTF8String:cocoa_widget_get_button_name(BUTTON_CANCEL)]];
  [myCancelButton setBezelStyle:(NSBezelStyle)1];
  [myButtonView addSubview:myOKButton];
  [myButtonView addSubview:myCancelButton];
  [myOKButton setKeyEquivalent:@"\r"];
  [myOKButton setNeedsDisplay:YES];
  [myViewView addSubview:myButtonView];
  [myColorPanel setColor:myDefCol];
  NSView *oldView = [myColorPanel contentView];
  [oldView retain];
  [myColorPanel setContentView:0];
  NSView *colorView = [[NSView alloc] initWithFrame:NSMakeRect(0, 0, myColorSize.width, myColorSize.height)];
  NSView *parentView = [[NSView alloc] initWithFrame:NSMakeRect(0, 0, myColorSize.width, myColorSize.height)];
  [colorView addSubview:oldView];
  [parentView addSubview:colorView];
  [parentView addSubview:myViewView];
  [myColorPanel setContentView:parentView];
  [[myColorPanel standardWindowButton:NSWindowCloseButton] setHidden:YES];
  [[myColorPanel standardWindowButton:NSWindowMiniaturizeButton] setHidden:YES];
  [[myColorPanel standardWindowButton:NSWindowZoomButton] setHidden:YES];
  myColorPanel.styleMask &= ~NSWindowStyleMaskResizable;
  colorView.autoresizingMask =  NSViewMinYMargin;
  parentView.autoresizingMask = NSViewMaxYMargin;
  myViewView.autoresizingMask = NSViewWidthSizable | NSViewMinXMargin;
  myButtonView.autoresizingMask = NSViewMinXMargin;

  NSModalSession colorSession = [NSApp beginModalSessionForWindow:myColorPanel];
  [myColorPanel setFrame:NSMakeRect(0, 0, 229, 399 + buttonHeight) display:YES animate:NO];
  [myColorPanel center];

  CGFloat r, g, b, a;
  NSColor *myColor = myDefCol;
  int redIntValue, greenIntValue, blueIntValue;
  NSString *redHexValue, *greenHexValue, *blueHexValue;
  NSColor *convertedColor;

  int rescol = -1;
  bool colorOKPressed = false;

  for (;;) {
    if ([NSApp runModalSession:colorSession] != NSModalResponseContinue)
      break;
    
    if ([[myColorPanel standardWindowButton:NSWindowCloseButton] state] == NSControlStateValueOn) {
      [[myColorPanel standardWindowButton:NSWindowCloseButton] setState:NSControlStateValueOff];
      [NSApp endModalSession:colorSession];
    }

    if ([myOKButton state] == NSControlStateValueOn) {
      [NSApp endModalSession:colorSession];
      colorOKPressed = true;
      [myColorPanel close];
    }

    if ([myCancelButton state] == NSControlStateValueOn) {
      [NSApp endModalSession:colorSession];
      [myColorPanel close];
    }
  }

  if (colorOKPressed) {
    myColor = [myColorPanel color];
    convertedColor = [myColor colorUsingType:NSColorTypeComponentBased];

    if (convertedColor) {
      [convertedColor getRed:&r green:&g blue:&b alpha:&a];
      redIntValue = (int)(r * 255);
      greenIntValue = (int)(g * 255);
      blueIntValue = (int)(b * 255);

      rescol = (redIntValue & 0xFF) + ((greenIntValue & 0xFF) << 8) + ((blueIntValue & 0xFF) << 16);
      [myColorPanel close];
    }
  }

  [myColorPanel setContentView:oldView];

  [oldView release];
  [colorView release];
  [myOKButton release];
  [myCancelButton release];
  [myButtonView release];
  [myViewView release];
  [parentView release];

  return rescol;
}

    string remove_trailing_zeros(double numb) {
      string strnumb = std::to_string(numb);
      while (!strnumb.empty() && strnumb.find('.') != string::npos && (strnumb.back() == '.' || strnumb.back() == '0'))
      strnumb.pop_back();
      return strnumb;
    }

    bool file_exists(string fname) {
      struct stat sb;
      return (stat(fname.c_str(), &sb) == 0 &&
        S_ISREG(sb.st_mode) != 0);
    }

    string filename_absolute(string fname) {
      char rpath[PATH_MAX];
      char *result = realpath(fname.c_str(), rpath);
      if (result != NULL) {
        if (file_exists(result)) return result;
      }
      return "";
    }

  } // anonymous namespace

  int show_message(char *str) {
    string str_str = str;
    return cocoa_show_message(str_str.c_str(), false, current_icon.c_str(), (caption == "") ? "Information" : caption.c_str());
  }

  int show_message_cancelable(char *str) {
    string str_str = str;
    return cocoa_show_message(str_str.c_str(), true, current_icon.c_str(), (caption == "") ? "Question" : caption.c_str());
  }

  int show_question(char *str) {
    string str_str = str;
    return cocoa_show_question(str_str.c_str(), false, current_icon.c_str(), (caption == "") ? "Question" : caption.c_str());
  }

  int show_question_cancelable(char *str) {
    string str_str = str;
    return cocoa_show_question(str_str.c_str(), true, current_icon.c_str(), (caption == "") ? "Question" : caption.c_str());
  }

  int show_attempt(char *str) {
    string str_str = str;
    return cocoa_show_attempt(str_str.c_str(), current_icon.c_str(), (caption == "") ? "Error" : caption.c_str());
  }

  int show_error(char *str, bool abort) {
    string str_str = str;
    int result = cocoa_show_error(str_str.c_str(), abort, current_icon.c_str(), (caption == "") ? "Error" : caption.c_str());
    if (result == 1) exit(0);
    return result;
  }

  char *get_string(char *str, char *def) {
    string str_str = str;
    string str_def = def;
    return (char *)cocoa_input_box(str_str.c_str(), str_def.c_str(), current_icon.c_str(), (caption == "") ? "Input Query" : caption.c_str(), false);
  }

  char *get_password(char *str, char *def) {
    string str_str = str;
    string str_def = def;
    return (char *)cocoa_password_box(str_str.c_str(), str_def.c_str(), current_icon.c_str(), (caption == "") ? "Input Query" : caption.c_str(), false);
  }

  double get_integer(char *str, double def) {
    double DIGITS_MIN = -999999999999999;
    double DIGITS_MAX = 999999999999999;

    if (def < DIGITS_MIN) def = DIGITS_MIN;
    if (def > DIGITS_MAX) def = DIGITS_MAX;

    string str_str = str;
    string str_def = remove_trailing_zeros(def);
    double result = strtod(cocoa_input_box(str_str.c_str(), str_def.c_str(), current_icon.c_str(), (caption == "") ? "Input Query" : caption.c_str(), true), NULL);

    if (result < DIGITS_MIN) result = DIGITS_MIN;
    if (result > DIGITS_MAX) result = DIGITS_MAX;

    return result;
  }

  double get_passcode(char *str, double def) {
    double DIGITS_MIN = -999999999999999;
    double DIGITS_MAX = 999999999999999;

    if (def < DIGITS_MIN) def = DIGITS_MIN;
    if (def > DIGITS_MAX) def = DIGITS_MAX;

    string str_str = str;
    string str_def = remove_trailing_zeros(def);
    double result = strtod(cocoa_password_box(str_str.c_str(), str_def.c_str(), current_icon.c_str(), (caption == "") ? "Input Query" : caption.c_str(), true), NULL);

    if (result < DIGITS_MIN) result = DIGITS_MIN;
    if (result > DIGITS_MAX) result = DIGITS_MAX;

    return result;
  }

  char *get_open_filename(char *filter, char *fname) {
    string str_filter = filter; string str_fname = fname; static string result;
    result = cocoa_get_open_filename(str_filter.c_str(), str_fname.c_str(), "", "", false);
    return (char *)result.c_str();
  }

  char *get_open_filename_ext(char *filter, char *fname, char *dir, char *title) {
    string str_filter = filter; string str_fname = fname;
    string str_dir = dir; string str_title = title; static string result;
    result = cocoa_get_open_filename(str_filter.c_str(), str_fname.c_str(), str_dir.c_str(), str_title.c_str(), false);
    return (char *)result.c_str();
  }

  char *get_open_filenames(char *filter, char *fname) {
    string str_filter = filter; string str_fname = fname; static string result;
    result = cocoa_get_open_filename(str_filter.c_str(), str_fname.c_str(), "", "", true);
    return (char *)result.c_str();
  }

  char *get_open_filenames_ext(char *filter, char *fname, char *dir, char *title) {
    string str_filter = filter; string str_fname = fname;
    string str_dir = dir; string str_title = title; static string result;
    result = cocoa_get_open_filename(str_filter.c_str(), str_fname.c_str(), str_dir.c_str(), str_title.c_str(), true);
    return (char *)result.c_str();
  }

  char *get_save_filename(char *filter, char *fname) {
    string str_filter = filter; string str_fname = fname; static string result;
    result = cocoa_get_save_filename(str_filter.c_str(), str_fname.c_str(), "", "");
    return (char *)result.c_str();
  }

  char *get_save_filename_ext(char *filter, char *fname, char *dir, char *title) {
    string str_filter = filter; string str_fname = fname;
    string str_dir = dir; string str_title = title; static string result;
    result = cocoa_get_save_filename(str_filter.c_str(), str_fname.c_str(), str_dir.c_str(), str_title.c_str());
    return (char *)result.c_str();
  }

  char *get_directory(char *dname) {
    string str_dname = dname;  static string result;
    result = cocoa_get_directory("", str_dname.c_str());
    return (char *)result.c_str();
  }

  char *get_directory_alt(char *capt, char *root) {
    string str_dname = root; string str_title = capt; static string result;
    result = cocoa_get_directory(str_title.c_str(), str_dname.c_str());
    return (char *)result.c_str();
  }

  int get_color(int defcol) {
    return cocoa_get_color(defcol, "");
  }

  int get_color_ext(int defcol, char *title) {
    string str_title = title;
    return cocoa_get_color(defcol, str_title.c_str());
  }

  char *widget_get_caption() {
    return (char *)caption.c_str();
  }

  void widget_set_caption(char *str) {
    caption = str;
  }

  char *widget_get_owner() {
    return cocoa_widget_get_owner();
  }

  void widget_set_owner(char *hwnd) {
    cocoa_widget_set_owner(hwnd);
  }

  char *widget_get_icon() {
    return (char *)current_icon.c_str();
  }

  void widget_set_icon(char *icon) {
    current_icon = filename_absolute(icon);
  }

  char *widget_get_system() {
    return (char *)"Cocoa";
  }

  void widget_set_system(char *sys) {

  }

  void widget_set_button_name(int type, char *name) {
    string path = (getenv("TMPDIR") ? : "/tmp");
    path += "/" + string(cocoa_get_bundle_id()) + "/";
    string fname1 = path + "libdlgmod.ini";
    string fname2 = path + "libdlgmod.tmp";
    mkdir(path.c_str(), 0777);
    std::ofstream ostrm;
    ostrm.open(fname2);
    if (ostrm) {
      ostrm << "[buttons]\n" <<
        "btn_abort = " << ((type == 0) ? name : widget_get_button_name(BUTTON_ABORT)) << "\n" <<
        "btn_ignore = " << ((type == 1) ? name : widget_get_button_name(BUTTON_IGNORE)) << "\n" <<
        "btn_ok = " << ((type == 2) ? name : widget_get_button_name(BUTTON_OK)) << "\n" <<
        "btn_cancel = " << ((type == 3) ? name : widget_get_button_name(BUTTON_CANCEL)) << "\n" <<
        "btn_yes = " << ((type == 4) ? name : widget_get_button_name(BUTTON_YES)) << "\n" <<
        "btn_no = " << ((type == 5) ? name : widget_get_button_name(BUTTON_NO)) << "\n" <<
        "btn_retry = " << ((type == 6) ? name : widget_get_button_name(BUTTON_RETRY)) << "\n";
    }
    if (file_exists(fname1))
      std::remove(fname1.c_str());
    std::rename(fname2.c_str(), fname1.c_str());
  }

  char *widget_get_button_name(int type) {
    string path = (getenv("TMPDIR") ? : "/tmp");
    path += "/" + string(cocoa_get_bundle_id()) + "/";
    string fname = path + "libdlgmod.ini";
    if (file_exists(fname)) {
      config cfg(fname);
      btn_array[BUTTON_ABORT] = cfg.get_value("buttons", "btn_abort");
      btn_array[BUTTON_IGNORE] = cfg.get_value("buttons", "btn_ignore");
      btn_array[BUTTON_OK] = cfg.get_value("buttons", "btn_ok");
      btn_array[BUTTON_CANCEL] = cfg.get_value("buttons", "btn_cancel");
      btn_array[BUTTON_YES] = cfg.get_value("buttons", "btn_yes");
      btn_array[BUTTON_NO] = cfg.get_value("buttons", "btn_no");
      btn_array[BUTTON_RETRY] = cfg.get_value("buttons", "btn_retry");
    }
    return (char *)btn_array[(int)type].c_str();
  }

