/*
 * Keynote.h
 */

#import <AppKit/AppKit.h>
#import <ScriptingBridge/ScriptingBridge.h>


@class KeynoteApplication, KeynoteDocument, KeynoteWindow, KeynoteTheme, KeynoteSlide;

enum KeynoteSaveOptions {
	KeynoteSaveOptionsYes = 'yes ' /* Save the file. */,
	KeynoteSaveOptionsNo = 'no  ' /* Do not save the file. */,
	KeynoteSaveOptionsAsk = 'ask ' /* Ask the user whether or not to save the file. */
};
typedef enum KeynoteSaveOptions KeynoteSaveOptions;

enum KeynotePrintingErrorHandling {
	KeynotePrintingErrorHandlingStandard = 'lwst' /* Standard PostScript error handling */,
	KeynotePrintingErrorHandlingDetailed = 'lwdt' /* print a detailed report of PostScript errors */
};
typedef enum KeynotePrintingErrorHandling KeynotePrintingErrorHandling;

enum KeynoteSaveableFileFormat {
	KeynoteSaveableFileFormatKeynote = 'Knff' /* The Keynote native file format */
};
typedef enum KeynoteSaveableFileFormat KeynoteSaveableFileFormat;

enum KeynoteExportFormat {
	KeynoteExportFormatHTML = 'Khtm' /* HTML */,
	KeynoteExportFormatMovie = 'Kmov' /* QuickTime movie */,
	KeynoteExportFormatPDF = 'Kpdf' /* PDF */,
	KeynoteExportFormatImage = 'Kimg' /* image */,
	KeynoteExportFormatPowerPoint = 'Kppt' /* PowerPoint */,
	KeynoteExportFormatClassic = 'Kkey' /* iWork Classic */
};
typedef enum KeynoteExportFormat KeynoteExportFormat;

enum KeynoteImageExportFormats {
	KeynoteImageExportFormatsJPEG = 'Kifj' /* JPEG */,
	KeynoteImageExportFormatsPNG = 'Kifp' /* PNG */,
	KeynoteImageExportFormatsTIFF = 'Kift' /* TIFF */
};
typedef enum KeynoteImageExportFormats KeynoteImageExportFormats;

enum KeynoteMovieExportFormats {
	KeynoteMovieExportFormatsSmall = 'Kmf3' /* 360p */,
	KeynoteMovieExportFormatsMedium = 'Kmf5' /* 540p */,
	KeynoteMovieExportFormatsLarge = 'Kmf7' /* 720p */
};
typedef enum KeynoteMovieExportFormats KeynoteMovieExportFormats;

enum KeynotePrintWhat {
	KeynotePrintWhatIndividualSlides = 'Kpwi' /* individual slides */,
	KeynotePrintWhatSlideWithNotes = 'Kpwn' /* slides with notes */,
	KeynotePrintWhatOutline = 'Kpwo' /* outline */,
	KeynotePrintWhatHandouts = 'Kpwh' /* handouts */
};
typedef enum KeynotePrintWhat KeynotePrintWhat;



/*
 * Standard Suite
 */

// The application's top-level scripting object.
@interface KeynoteApplication : SBApplication

- (SBElementArray *) documents;
- (SBElementArray *) windows;

@property (copy, readonly) NSString *name;  // The name of the application.
@property (readonly) BOOL frontmost;  // Is this the active application?
@property (copy, readonly) NSString *version;  // The version number of the application.

- (id) open:(id)x;  // Open a document.
- (void) print:(id)x withProperties:(NSDictionary *)withProperties printDialog:(BOOL)printDialog;  // Print a document.
- (void) quitSaving:(KeynoteSaveOptions)saving;  // Quit the application.
- (BOOL) exists:(id)x;  // Verify that an object exists.
- (void) showNext;  // Advance one build or slide.
- (void) showPrevious;  // Go to the previous slide.

@end

// A document.
@interface KeynoteDocument : SBObject

@property (copy, readonly) NSString *name;  // Its name.
@property (readonly) BOOL modified;  // Has it been modified since the last save?
@property (copy, readonly) NSURL *file;  // Its location on disk, if it has one.

- (void) closeSaving:(KeynoteSaveOptions)saving savingIn:(NSURL *)savingIn;  // Close a document.
- (void) saveIn:(NSURL *)in_ as:(KeynoteSaveableFileFormat)as;  // Save a document.
- (void) printWithProperties:(NSDictionary *)withProperties printDialog:(BOOL)printDialog;  // Print a document.
- (void) delete;  // Delete an object.
- (void) duplicateTo:(SBObject *)to withProperties:(NSDictionary *)withProperties;  // Copy an object.
- (void) moveTo:(SBObject *)to;  // Move an object to a new location.
- (void) exportTo:(NSURL *)to as:(KeynoteExportFormat)as withProperties:(NSDictionary *)withProperties;  // Export a slideshow to another file
- (void) startFrom:(KeynoteSlide *)from;  // Start playing the slideshow.
- (void) stop;  // Stop the slideshow.
- (void) showSlideSwitcher;  // Show the slide switcher in play mode
- (void) hideSlideSwitcher;  // Hide the slide switcher in play mode
- (void) moveSlideSwitcherForward;  // Move the slide switcher forward one slide
- (void) moveSlideSwitcherBackward;  // Move the slide switcher backward one slide
- (void) cancelSlideSwitcher;  // Hide the slide switcher without changing slides
- (void) acceptSlideSwitcher;  // Hide the slide switcher, going to the slide it has selected

@end

// A window.
@interface KeynoteWindow : SBObject

@property (copy, readonly) NSString *name;  // The title of the window.
- (NSInteger) id;  // The unique identifier of the window.
@property NSInteger index;  // The index of the window, ordered front to back.
@property NSRect bounds;  // The bounding rectangle of the window.
@property (readonly) BOOL closeable;  // Does the window have a close button?
@property (readonly) BOOL miniaturizable;  // Does the window have a minimize button?
@property BOOL miniaturized;  // Is the window minimized right now?
@property (readonly) BOOL resizable;  // Can the window be resized?
@property BOOL visible;  // Is the window visible right now?
@property (readonly) BOOL zoomable;  // Does the window have a zoom button?
@property BOOL zoomed;  // Is the window zoomed right now?
@property (copy, readonly) KeynoteDocument *document;  // The document whose contents are displayed in the window.

- (void) closeSaving:(KeynoteSaveOptions)saving savingIn:(NSURL *)savingIn;  // Close a document.
- (void) saveIn:(NSURL *)in_ as:(KeynoteSaveableFileFormat)as;  // Save a document.
- (void) printWithProperties:(NSDictionary *)withProperties printDialog:(BOOL)printDialog;  // Print a document.
- (void) delete;  // Delete an object.
- (void) duplicateTo:(SBObject *)to withProperties:(NSDictionary *)withProperties;  // Copy an object.
- (void) moveTo:(SBObject *)to;  // Move an object to a new location.

@end



/*
 * Keynote Suite
 */

@interface KeynoteTheme : SBObject

@property (copy, readonly) NSString *name;

- (void) closeSaving:(KeynoteSaveOptions)saving savingIn:(NSURL *)savingIn;  // Close a document.
- (void) saveIn:(NSURL *)in_ as:(KeynoteSaveableFileFormat)as;  // Save a document.
- (void) printWithProperties:(NSDictionary *)withProperties printDialog:(BOOL)printDialog;  // Print a document.
- (void) delete;  // Delete an object.
- (void) duplicateTo:(SBObject *)to withProperties:(NSDictionary *)withProperties;  // Copy an object.
- (void) moveTo:(SBObject *)to;  // Move an object to a new location.

@end

// A slide in a slideshow document
@interface KeynoteSlide : SBObject

- (void) closeSaving:(KeynoteSaveOptions)saving savingIn:(NSURL *)savingIn;  // Close a document.
- (void) saveIn:(NSURL *)in_ as:(KeynoteSaveableFileFormat)as;  // Save a document.
- (void) printWithProperties:(NSDictionary *)withProperties printDialog:(BOOL)printDialog;  // Print a document.
- (void) delete;  // Delete an object.
- (void) duplicateTo:(SBObject *)to withProperties:(NSDictionary *)withProperties;  // Copy an object.
- (void) moveTo:(SBObject *)to;  // Move an object to a new location.

@end

// A Keynote slideshow document.
@interface KeynoteDocument (KeynoteSuite)

- (SBElementArray *) slides;

@property (copy, readonly) KeynoteTheme *theme;

@end

// The Keynote application.
@interface KeynoteApplication (KeynoteSuite)

- (SBElementArray *) themes;


@end

