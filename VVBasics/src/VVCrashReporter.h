/*
	it's been my experience that most apps crash much more frequently on end-users than the app's 
	developers would guess.  the simplest and easiest way to improve the end-user's experience is to 
	have the application check their machine for crash logs- which are generated automatically by os 
	x whenever an application crashes- and send them to the developer.
	
	this class exists so i have a really easy way to make my apps send their crash logs to me; the 
	goal here is to make it as easy as possible to get all the information i need to troubleshoot a 
	problem with as little work on the user's part as possible.  it also sends a basic system 
	profile, anything the app recently printed to the console log, and optional description/email 
	fields to facilitate communication directly with the user.  this data is then uploaded to a 
	given URL using an HTTP POST.
	
	on the server side, i use a PHP page which sanitizes the POST data (i can't stress how important 
	this step is) and works with it.  i've included a sample PHP page that simply dumps the received 
	data to a file on disk (and optionally emails someone) with this project.
	
	HOW TO USE THIS CLASS:
	
		1)- create an instance of this class
		
		2)- set the instance's delegate and uploadURL.  these are necessary!
		
		3)- call "check" on the instance.  when it's done, it calls "crashReporterCheckDone" on the 
			delegate.
*/


#import <Cocoa/Cocoa.h>
#import <SystemConfiguration/SCNetwork.h>
#import <asl.h>
#import "VVCURLDL.h"
#import "MutLockArray.h"




@protocol VVCrashReporterDelegate
- (void) crashReporterCheckDone;
@end




@interface VVCrashReporter : NSObject <VVCURLDLDelegate> {
	NSString						*uploadURL;	//	does NOT includes http://
	id								delegate;	//	must respond to VVCrashReporterDelegate protocol
	MutLockArray					*crashLogArray;
	NSMutableDictionary				*systemProfilerDict;
	NSString						*consoleLog;
	int								jobSize;			//	used to update progress indicator/label
	int								jobCurrentIndex;	//	used to update progress indicator/label
	int								currentCrashLogTimeout;	//	countdown for timeout of sending/receiving data for a specific crash log
	NSTimer							*currentCrashLogTimer;
	
	IBOutlet NSWindow				*window;
	IBOutlet NSTextField			*emailField;
	IBOutlet NSTextView				*descriptionField;
	IBOutlet NSTextField			*submittingLabel;	//	non-editable. 'submitting', 'getting machine profile', etc.
	IBOutlet NSProgressIndicator	*progressIndicator;	//	indicates progress through all crash logs to be submitted
	IBOutlet NSTextField			*countdownLabel;	//	non-editable; countdown so user knows app hasn't hung
	
	NSNib							*theNib;
	NSArray							*nibTopLevelObjects;
}

- (void) check;
- (void) openCrashReporter;
- (IBAction) doneClicked:(id)sender;
- (void) sendACrashLog;
- (void) closeCrashReporter;

- (NSString *) _nibName;
- (NSString *) _consoleLogString;
- (NSMutableDictionary *) _systemProfilerDict;
- (NSString *) _stringForSystemProfilerDataType:(NSString *)t;

- (void) updateCrashLogTimeout:(NSTimer *)t;

//	VVCURLDLDelegate method- this class will be the delegate of multiple VVCURLDL instances
- (void) dlFinished:(id)h;

- (void) setUploadURL:(NSString *)n;
- (NSString *) uploadURL;

@property (assign,readwrite) id delegate;

@end
