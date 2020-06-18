/*!
 * Lateral Slider v1.0
 * May only be used if purchased.
 * Refer to licensing details at <http://codecanyon.net/wiki/support/legal-terms/licensing-terms/>.
 * Find product at <http://codecanyon.net/user/nviswan10/portfolio?ref=nviswan10>.
 *
 * Copyright 2010, Karthik Viswanathan
 * <http://www.lateralcode.com>
 * <http://twitter.com/lateralcode/>
 */

// anonymous, self-invoking function to have a local scope and to prevent conflicts with other libraries
( function( $ ) {
	
	// simple javascript inheritance - http://ejohn.org/blog/simple-javascript-inheritance/
	// inspired by base2 and Prototype
	( function() {
	  	var initializing = false;
		var fnTest = ( /xyz/.test( function() { xyz; } ) ) ? ( /\b_super\b/ ) : ( /.*/ );

	  	// the base class implementation (does nothing)
	  	this.Class = function() {};

	  	// create a new class that inherits from this class
	  	Class.extend = function(prop) {
	    	var _super = this.prototype;

	    	// instantiate a base class (but only create the instance,
	    	// don't run the init constructor)
	    	initializing = true;
	    	var prototype = new this();
	    	initializing = false;

	    	// copy the properties over onto the new prototype
	    	for (var name in prop) {
	      
				// check if we're overwriting an existing function
	      		if ( typeof prop[name] == "function" && typeof _super[name] == "function" 
					&& fnTest.test(prop[name]) ) {
					
					prototype[name] = ( function( name, fn ) {
						return function() {
			            	var tmp = this._super;

			            	// add a new ._super() method that is the same method
			            	// but on the super-class
			            	this._super = _super[name];

			            	// the method only needs to be bound temporarily, so we
			            	// remove it when we're done executing
			            	var ret = fn.apply( this, arguments );       
			            	this._super = tmp;

			            	return ret;
			          	};
			        } )( name, prop[name] );
				}
				else
	       			prototype[name] = prop[name];
	    	}

	    	// the dummy class constructor
	    	function Class() {
	      		// all construction is actually done in the init method
	      		if ( !initializing && this.init )
	        		this.init.apply(this, arguments);
	    	}

	    	// populate our constructed prototype object
	    	Class.prototype = prototype;

	    	// enforce the constructor to be what we expect
	    	Class.constructor = Class;

	    	// and make this class extendable
	    	Class.extend = arguments.callee;

	    	return Class;
	  	};
	} )();
	// end simple javascript inheritance
	
	// extend jQuery with the new lateralSlider function
	$.fn.lateralSlider = function( options ) {
		
		// default options that are used sans parameters
		var defaults = {
			displayDuration: 2000, // how long to display a slide
			animateDuration: 1500, // how long to animate (transition to the next slide)
			numColumns: 10, // number of columns (slices) used for animation
			transitions:  "fade,slideUp,slideDown,slideLeft,slideRight,slideUpAndDown,slideLeftAndRight,fadeAndSlideUp,fadeAndSlideDown,fadeAndSlideLeft,fadeAndSlideRight,fadeSlideUpAndRight,fadeSlideDownAndLeft", // list of comma-separated transitions to use
			random: false, // whether to randomize the transitions
			hidePrevAndNextArrows: false, // whether to hide the navigation arrows (previous slide and next slide)
			hideSlideChooser: false, // whether to hide the slide chooser (bottom right hand corner)
			captionOpacity: 0.8 // caption transparency (0 fully transparent, 1 fully opaque)
		};
		
		// find the intersection of options, overriding 
		// the default with the user's input
		var settings = $.extend( {}, defaults, options );
		
		// process all sliders
		this.each( function() {
			// cache of jQuery $(this)... also used out of context
			var $this = $(this);
			
			// slideshow class which controls the whole show and 
			// transitions
			var SlideShow = Class.extend( {
				// images inside $this and the number of images
				$imgs : null, size : null,
				
				// what image is being displayed and the upcoming image
				displayImg : null, nextImg : null,
				
				// the number of divs (columns/slices) to use and a single div width
				numDivs : null, divWidth : null,
				
				// the css for each div along with the jQuery object for all divs
				baseCSS : null, $divs : null,
				
				// transitions to use and the current transition
				transitions : [], transition : null,
				
				// transition to use if settings.random is set to false
				transitionCount : null,
				
				// the interval, returned by setInterval, which cycles the images
				interval : null,
				
				// the width of $this
				width : null,
				
				// constructor
				init: function() {
					// initializations
					this.$imgs = $( 'img', $this );
					this.size = this.$imgs.size();
					
					this.$imgs.hide();
					this.nextImg = 0;
					
					// stored because the width is constant
					this.width = $this.width();
					
					this.numDivs = settings.numColumns;
					this.divWidth = this.width / this.numDivs;
					
					this.baseCSS = {
						width: this.divWidth,
						position: 'absolute', top: 0,
						backgroundRepeat: 'no-repeat'
					};
					
					// div creation used for animation
					this.createDivs();
					this.$divs = $( 'div', $this );
					
					// start at negative due to initial increment in the getTransition function
					this.transitionCount = -1;
				},
				
				// helper function to create all the divs
				createDivs : function() {
					for( var i = 0; i < this.numDivs; i++ )
					{
						var div = $( '<div></div>' );

						// apply the css and the unique left value which determines position
						div.css( this.baseCSS );
						div.css( 'left', this.divWidth * i );
						
						// append after css is applied
						div.appendTo( $this );
					}
				}
				
			} );
			
			// the show, which is referenced in the transitions
			var show = new SlideShow();
			
			// the base transition class which is extended for customization
			var Transition = Class.extend( {
				// animation duration
				baseDuration : null,
				
				// additional duration (constant)
				originalOffset: null,
				
				// additional duration (variable)
				offset: null,
				
				// constructor
				init : function() {
					this.baseDuration = settings.animateDuration / 8;
					
					// offset will be added numDivs times, so that 
					// baseDuration + numDivs * offset = settings.animateDuration
					this.originalOffset = 7 * settings.animateDuration / ( 8 * show.numDivs );
					this.offset = 7 * settings.animateDuration / ( 8 * show.numDivs );
				},
				
				// current duration, increment duration, and reset duration
				duration: function() { return this.baseDuration + this.offset; },
				increment: function() { this.offset += this.originalOffset; },
				reset: function() { this.offset = this.originalOffset; },
				
				// functions to be overriden in sub-classes
				getCSS : function( div ) { return {}; },
				eachDiv : function() { return {}; },
				
				// basic apply transition, which applies eachDiv to each div element
				applyTransition : function() {
					show.$divs.each( this.eachDiv );
					this.reset();
				}
			} );
			
			// transition which uses the jQuery animate function
			var AnimationTransition = Transition.extend( {
				// override
				applyTransition : function() {
					// this stored to use in different context
					var oldThis = this;
					
					show.$divs.each( function() {
						// eachDiv can be an object with css values or a function
						var css = oldThis.eachDiv;
						if( typeof( oldThis.eachDiv ) == "function" )
							css = css();
						
						$(this).animate( css, oldThis.duration() );
						oldThis.increment(); // change duration to create complex transitions
					} );
					
					// reset duration for the next time this transition is called
					this.reset();
				}
			} );
			
			// compound transition which involves multiple animation transitions
			var CompoundTransition = AnimationTransition.extend( {
				// index to use in the arrays of getCSS/eachDiv functions
				getCSSIndex : null,
				eachDivIndex : null,
				
				// arrays of getCSS/eachDiv functions
				getCSSGroup : null,
				eachDivGroup : null,
				
				// constructor
				init : function() {
					this._super(); // transition's constructor
					
					this.getCSSGroup = new Array();
					this.eachDivGroup = new Array();
					
					this.getCSSIndex = 0;
					this.eachDivIndex = 0;
				},
				
				// cycle through getCSS array
				getCSS : function( div ) {
					var css = this.getCSSGroup[ this.getCSSIndex ];
					this.getCSSIndex = ( this.getCSSIndex + 1 ) % this.getCSSGroup.length;
					
					return css( div );
				},
				
				// add transition functions to array
				addTransition : function( transition ) {
					this.getCSSGroup.push( transition.getCSS );
					this.eachDivGroup.push( transition.eachDiv );
				},
				
				// cycle through eachDiv array
				applyTransition : function() {
					var oldThis = this; // change of context below
					
					show.$divs.each( function() {
						// sometimes eachDiv is a function rather than an object
						var animation = oldThis.eachDivGroup[ oldThis.eachDivIndex ];
						if( typeof( animation ) == "function" )
							animation = animation();
						
						$(this).animate( animation, oldThis.duration() );
						
						oldThis.eachDivIndex = ( oldThis.eachDivIndex + 1 ) % oldThis.eachDivGroup.length;
						oldThis.increment();
					} );
					
					// debug: console.log( "Done!" );
					this.reset();
				}
			} );
			
			// basic fade transition
			var FadeTransition = AnimationTransition.extend( {
				// fading starts at 0 opacity (hidden)
				getCSS : function( div ) { return { opacity: 0 }; },
				
				// fading goes from 0 => 1
				eachDiv : { opacity: 1 }
			} );
			
			// basic slide up transition
			var SlideUpTransition = AnimationTransition.extend( {
				// start under the #slider div (overflow hidden)
				getCSS : function( div ) { return { top: show.$imgs.eq( show.nextImg ).height() }; },
				
				// move to top: 0
				eachDiv : { top: 0 }
			} );
			
			// basic slide down transition
			var SlideDownTransition = AnimationTransition.extend( {
				// sliding starts at 0 height
				getCSS : function( div ) { return { height: 0 }; },
				
				// height goes from 0 to the image height
				// function because it must be calculated each time it's called (dynamic height)
				eachDiv : function() { return { height: show.$imgs.eq( show.nextImg ).height() }; }
			} );
			
			// basic slide left transition
			var SlideLeftTransition = AnimationTransition.extend( {
				// start with 0 width and transpose to the rightmost point
				getCSS : function( div ) {
					var left = div.css( 'left' );
					// extract number (left is a string that ends with "px")
					left = parseInt( left.substring( 0, left.length - 2 ), 10 );
					
					return {
						left: left + show.divWidth,
						width: 0
					};
				},
				
				// animate to full width while moving left
				eachDiv : { left: "-=" + show.divWidth, width: show.divWidth }
			} );
			
			// basic slide right transition
			var SlideRightTransition = AnimationTransition.extend( {
				// start with 0 width
				getCSS : function( div ) { return { width: 0 }; },
				
				// animate to full width
				eachDiv : { width: show.divWidth }
			} );
			
			// compound transition to slide up and down
			var SlideUpAndDownTransition = CompoundTransition.extend( {
				addTransitions: function( SlideUp, SlideDown ) {
					this.addTransition( SlideUp );
					this.addTransition( SlideDown );
				}
			} );
			
			// compound transition to slide left and right
			var SlideLeftAndRightTransition = CompoundTransition.extend( {
				addTransitions: function( SlideLeft, SlideRight ) {
					this.addTransition( SlideLeft );
					this.addTransition( SlideRight );
				}
			} );
			
			// compound transition to fade and slide up
			var FadeAndSlideUpTransition = CompoundTransition.extend( {
				// note transposed order to create a more visible difference
				addTransitions: function( SlideUp, Fade ) {
					this.addTransition( SlideUp );
					this.addTransition( Fade );
				}
			} );
			
			// compound transition to fade and slide down
			var FadeAndSlideDownTransition = CompoundTransition.extend( {
				addTransitions: function( Fade, SlideDown ) {
					this.addTransition( Fade );
					this.addTransition( SlideDown );
				}
			} );
			
			// compound transition to fade and slide left
			var FadeAndSlideLeftTransition = CompoundTransition.extend( {
				addTransitions: function( Fade, SlideLeft ) {
					this.addTransition( Fade );
					this.addTransition( SlideLeft );
				}
			} );
			
			// compound transition to fade and slide right
			var FadeAndSlideRightTransition = CompoundTransition.extend( {
				addTransitions: function( SlideRight, Fade ) {
					this.addTransition( SlideRight );
					this.addTransition( Fade );
				}
			} );
			
			// compound transition to fade, slide up, and slide right
			var FadeSlideUpAndRightTransition = CompoundTransition.extend( {
				addTransitions: function( SlideUp, Fade, SlideRight ) {
					this.addTransition( SlideUp );
					this.addTransition( Fade );
					this.addTransition( SlideRight );
				}
			} );
			
			// compound transition to fade, slide down, and slide left
			var FadeSlideDownAndLeftTransition = CompoundTransition.extend( {
				addTransitions: function( SlideDown, Fade, SlideLeft ) {
					this.addTransition( SlideDown );
					this.addTransition( Fade );
					this.addTransition( SlideLeft );
				}
			} );
			
			// map of transitions to use
			var map = {
				fade : new FadeTransition(), // 1
				slideUp : new SlideUpTransition(), // 2
				slideDown : new SlideDownTransition(), // 3
				slideLeft : new SlideLeftTransition(), // 4
				slideRight : new SlideRightTransition(), // 5
				slideUpAndDown : new SlideUpAndDownTransition(), // 6
				slideLeftAndRight : new SlideLeftAndRightTransition(), // 7
				fadeAndSlideUp : new FadeAndSlideUpTransition(), // 8
				fadeAndSlideDown : new FadeAndSlideDownTransition(), // 9
				fadeAndSlideLeft : new FadeAndSlideLeftTransition(), // 10
				fadeAndSlideRight : new FadeAndSlideRightTransition(), // 11
				fadeSlideUpAndRight : new FadeSlideUpAndRightTransition(), // 12
				fadeSlideDownAndLeft : new FadeSlideDownAndLeftTransition() // 13
			};
			
			// call the addTransitions function with all compound transitions
			map.slideUpAndDown.addTransitions( map.slideUp, map.slideDown );
			map.slideLeftAndRight.addTransitions( map.slideLeft, map.slideRight );
			
			// parameters alternate for these transitions to have transposed order
			map.fadeAndSlideUp.addTransitions( map.slideUp, map.fade );
			map.fadeAndSlideDown.addTransitions( map.fade, map.slideDown );
			
			map.fadeAndSlideLeft.addTransitions( map.fade, map.slideLeft );
			map.fadeAndSlideRight.addTransitions( map.slideRight, map.fade );
			
			map.fadeSlideUpAndRight.addTransitions( map.slideUp, map.fade, map.slideRight );
			map.fadeSlideDownAndLeft.addTransitions( map.slideDown, map.fade, map.slideLeft );
			
			// populate the transitions array based off of settings.transitions
			SlideShow.prototype.populateTransitions = function() {
				// split the string and use the map to add transitions
				var names = settings.transitions.split( /,\s*/g );
				// debug: console.log( names );
				for( var index in names )
					this.transitions.push( map[ names[ index ] ] );
				// debug: console.log( this.transitions );
			};
			
			// get the next transition to use
			SlideShow.prototype.getTransition = function() {
				// if random, generate random index
				if( settings.random ) {
					var index = Math.floor( Math.random() * this.transitions.length );
					return this.transitions[ index ];
				}
				// otherwise just cycle through
				else {
					this.transitionCount = ( this.transitionCount + 1 ) % this.transitions.length;
					return this.transitions[ this.transitionCount ];
				}
			};
			
			// adding necessary css to each div
			SlideShow.prototype.addDivCSS = function() {
				var oldThis = this;
				
				// upcoming image to display
				var $img = oldThis.$imgs.eq( oldThis.nextImg );
				
				// used as css for each div
				var image = 'url(' + $img.attr( 'src' ) + ')';
				var height = $img.height();
				
				this.$divs.each( function() {
					var $div = $(this);
				
					// background image, position, and height change depending on the image
					$div.css( {
						backgroundImage: image,
						backgroundPosition: '-' + $div.css( 'left' ) + ' 0px',
						height: height
					} );
				
					// transition css
					$div.css( oldThis.transition.getCSS( $div ) );
				} ); 
			};

			// process the next image and display it
			SlideShow.prototype.process = function() {
				// set the background to the current image
				$this.css( {
					backgroundImage: 'url(' + this.$imgs.eq( this.displayImg ).attr( 'src' ) + ')',
					backgroundRepeat: 'no-repeat'
				} );
				
				// add new css to the divs and apply the transition
				this.transition = this.getTransition();
				// debug: console.log( "Applying css...");
				this.addDivCSS();
				// debug: console.log( "Starting transition..." );
				this.transition.applyTransition();
				
				$this.animate( {
					// height should be based off of the image that will be transitioned to
					height: this.$imgs.eq( this.nextImg ).height()
				}, settings.animateDuration );
				
				// update slide chooser
				this.advanceShow();
			};
			
			// used to update the slide chooser
			SlideShow.prototype.updateCurrent = function() {
				// based off of rel attribute
				$( '.circle[rel="' + this.displayImg + '"]' ).removeClass( 'circle-current' );
				$( '.circle[rel="' + this.nextImg + '"]' ).addClass( 'circle-current' );
			};
			
			// advance the show
			SlideShow.prototype.advanceShow = function() {
				this.updateCurrent();
				this.displayImg = this.nextImg;
				
				// move to next image or loop back to start
				if( this.nextImg == this.size - 1 )
					this.nextImg = 0;
				else
					this.nextImg++;
			};
			
			// call window.setInterval to begin the show
			SlideShow.prototype.startShow = function() {
				this.interval = window.setInterval( $.proxy( this.runner, this ), settings.displayDuration + settings.animateDuration );
			};
			
			// clear the interval to stop the show
			SlideShow.prototype.stopShow = function() {
				window.clearInterval( this.interval );
			};
			
			// go to a specific slide in the show
			SlideShow.prototype.goToSlide = function( index ) {
				// if a transition is going on, don't interrupt it
				if( this.$divs.filter( ':animated' ).size() > 0 )
					return;
				
				// stop and update image
				this.stopShow();
				this.nextImg = index;
				
				this.updateCurrent();
				this.runner();
				
				// take care of bounds
				if( this.nextImg == 0 )
					this.displayImg = this.size - 1;
				else
					this.displayImg = this.nextImg - 1;
				
				// restart
				this.startShow();
			};
			
			// used to apply an anchor tag (link) if the image is enclosed by one
			SlideShow.prototype.applyLink = function() {
				// remove previous link
				var $display = this.$imgs.eq( this.displayImg );
				var $displayParent = $display.parent();
				
				if( $displayParent.is( 'a' ) )
					// simple way to get rid of all jQuery styling
					$displayParent.removeAttr( 'style' );
				
				// add next link
				var $next = this.$imgs.eq( this.nextImg );
				var $nextParent = $next.parent();
				
				if( $nextParent.is( 'a' ) ) {
					// debug: console.log( "Adding styling for link..." );
					
					// add styling
					$nextParent.css( {
						// block to give it dimensions, no underline or border
						display: 'block', textDecoration: 'none', border: '0', 
						
						// dimensions of the #slider div
						width: $this.width(), height: $next.height(),
						
						// positioned to fully cover it
						position: 'absolute', top: 0, left: 0, zIndex: 100
					} );
				}
			};
			
			// used to apply a caption based off of the title attribute
			SlideShow.prototype.applyCaption = function() {
				// get the title and caption element
				var $next = this.$imgs.eq( this.nextImg );
				var title = $next.attr( 'title' );
				
				var $caption = $( '.caption', $this );
				
				// remove the caption
				$caption.slideUp( function() {
					// callback to add the new title
					$(this).html( title );
				} );
				
				// if a new caption needs to be applied
				if( title != '' ) {
					
					// caption div already exists
					if( $caption.size() > 0 ) {
						// change the html if necessary (if visible, callback function above will take care of it)
						if( !$caption.is( ':visible' ) )
							$caption.html( title );
						
						// make it visible
						$caption.slideDown();
					}
					else {
						// debug: console.log( "Adding caption" );
						
						// make the new caption div
						var $newCaption = $( '<div class="caption"><span>' + title + '</span></div>' );
						
						$newCaption.css( {
							// defaults to faded background
							// note that opacity is set using jQuery so that it works in IE
							opacity: settings.captionOpacity,
							
							// same width as #slider
							width: $this.width(),
							
							// top left position
							position: 'absolute', top: 0, left: 0,
							
							// default to not displayed
							display: 'none'
						} );
						
						// show it
						$newCaption.appendTo( $this );
						$newCaption.slideDown();
					}
				}
			};
			
			// runner that is called periodically
			SlideShow.prototype.runner = function() {
				this.applyLink();
				this.applyCaption();
				
				// transition after link and caption have been applied
				this.process();
			};
			
			// begin the slideshow
			SlideShow.prototype.begin = function() {
				var $nextImg = this.$imgs.eq( this.nextImg );
				
				// set the right background image
				$this.css( {
					backgroundImage: 'url(' + $nextImg.attr( 'src' ) + ')',
					height: $nextImg.height()
				} );

				// call the runner to apply the link and caption (if they exist)
				this.runner();
				
				// restart at first transition (runner increments it) and start the show
				this.transitionCount--;
				this.startShow();
			};
			
			// class that adds the slideshow controls (slide chooser and previous/next buttons)
			var SlideShowControls = Class.extend( {
				// count of circles that are used in the slide chooser
				circleCount : null,
				
				// constructor
				init: function() {
					this.circleCount = 0;
				},
				
				// add all the controls
				addAll : function() {
					this.addCircles();
					this.addPrevAndNextLinks();
					
					// apply handlers
					$( 'a.circle' ).click( this.circleClickHandler );
					$( '.prev-link' ).click( this.prevLinkHandler );
					$( '.next-link' ).click( this.nextLinkHandler );
					
					// hide controls based off of the settings
					if( settings.hideSlideChooser )
						$( 'a.circle' ).hide();
					
					if( settings.hidePrevAndNextArrows )
						$( '.prev-link, .next-link' ).hide();
				},
				
				// helper function to add the circles
				addCircles : function() {
					var oldThis = this; // context change below
					
					show.$imgs.each( function() {
						// rel determines corresponding slide
						var circle = $( '<a href="#" rel="' + oldThis.circleCount + '" class="circle"></a>' );
						// space each circle apart
						circle.css( { right: ( show.size - oldThis.circleCount - 1 ) * 20 + 10 } );

						circle.appendTo( $this );
						oldThis.circleCount++;
					} );
				},
				
				// adding the previous and next link controls
				addPrevAndNextLinks : function() {
					$( '<a href="#" class="prev-link"></a>' ).appendTo( $this );
					$( '<a href="#" class="next-link"></a>' ).appendTo( $this );
				},
				
				// handler for the slide chooser click event
				circleClickHandler : function( event ) {
					var index = parseInt( $(this).attr( 'rel' ), 10 );
					show.goToSlide( index );

					// prevent normal click action
					event.preventDefault();
				},
				
				// handler for the previous link
				prevLinkHandler : function( event ) {
					// check bounds for negative index
					var index = show.displayImg - 1;
					if( index < 0 )
						index = show.size - 1;

					show.goToSlide( index );
					event.preventDefault();
				},
				
				// handler for the next link
				nextLinkHandler : function( event ) {
					// check bounds for high index
					var index = show.displayImg + 1;
					if( index >= show.size )
						index = 0;

					show.goToSlide( index );		
					event.preventDefault();
				}
			} );
			
			// create the controls and add them
			var controls = new SlideShowControls();
			controls.addAll();
			
			// populate the transition array and begin the show
			show.populateTransitions();
			show.begin();
		} );
		
		// return the jQuery object for chainability
		return this;
	};
	
} )( jQuery );