import { ComponentFixture, TestBed } from '@angular/core/testing';

import { AnimatedSvgComponent } from './animated-svg.component';

describe('AnimatedSvgComponent', () => {
  let component: AnimatedSvgComponent;
  let fixture: ComponentFixture<AnimatedSvgComponent>;

  beforeEach(async () => {
    await TestBed.configureTestingModule({
      declarations: [ AnimatedSvgComponent ]
    })
    .compileComponents();
  });

  beforeEach(() => {
    fixture = TestBed.createComponent(AnimatedSvgComponent);
    component = fixture.componentInstance;
    fixture.detectChanges();
  });

  it('should create', () => {
    expect(component).toBeTruthy();
  });
});
