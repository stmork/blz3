/* Generated with YAKINDU statechart tools
 *
 * SPDX-License-Identifier: BSD-3-Clause
 * SPDX-FileCopyrightText:  Copyright (C)  2024  Steffen A. Mork
 *               All rights reserved */

#ifndef SC_RXCPP_H_
#define SC_RXCPP_H_

#include "sc_types.h"
#include <vector>
#include <algorithm>
#include <functional>

namespace sc
{
	namespace rx
	{

		template<class T> class Observer;
		template<> class Observer<void> ;
		template<class T> class subscription;
		template<class T> class Observable;
		template<> class Observable<void> ;

		template<class T>
		class Observer
		{
		public:
			virtual ~Observer()
			{
			}
			virtual void next(T value) = 0;
		};

		template<>
		class Observer<void>
		{
		public:
			virtual ~Observer()
			{
			}
			virtual void next() = 0;
		};

		template<class T>
		class SingleSubscriptionObserver: public Observer<T>
		{
		public:
			SingleSubscriptionObserver() noexcept
			{
			}

			bool subscribe(sc::rx::Observable<T> & o) noexcept
			{
				return o.subscribe(*subscription);
			}

			bool unsubscribe(sc::rx::Observable<T> & o) noexcept
			{
				return o.unsubscribe(*subscription);
			}

			void setSubscription(sc::rx::subscription<T> & s) noexcept
			{
				subscription = &s;
			}

			virtual ~SingleSubscriptionObserver() = default;


		protected:
			sc::rx::subscription<T> * subscription;
		};

		template<class T> class ObservableBase;

		template<class T>
		class subscription
		{
		protected:
			Observer<T> * observer;
			subscription<T> * next;
			friend class ObservableBase<T>;
			friend class Observable<T>;
		public:
			subscription()  noexcept :
				observer(nullptr), next(nullptr)
			{
			}

			subscription(Observer<T> & o) noexcept :
				observer(&o), next(nullptr)
			{
			}

			bool operator==(const subscription & other) const  noexcept
			{
				return this == &other;
			}

			bool operator!=(const subscription & other) const  noexcept
			{
				return !(*this == other);
			}

			Observer<T> & operator*() const  noexcept
			{
				return *observer;
			}

			Observer<T> * operator->() const  noexcept
			{
				return observer;
			}

			operator bool() const  noexcept
			{
				return observer != nullptr;
			}

			virtual ~subscription() = default;
		};

		template<class T> class ObservableBase
		{
		public:
			ObservableBase()  noexcept :
				subscriptions(nullptr)
			{
			}

			bool subscribe(subscription<T> & s) noexcept
			{
				if (s && s.next == nullptr)
				{
					subscription<T> * currentSub = this->subscriptions;
					s.next = (currentSub != nullptr) ? currentSub : &s;
					this->subscriptions = &s;
					return true;
				}
				return false;
			}

			bool unsubscribe(subscription<T> & s) noexcept
			{
				if (this->subscriptions != nullptr)
				{
					if (*(this->subscriptions) == s)
					{
						this->subscriptions = (*(s.next) != s) ? s.next : nullptr;
						s.next = nullptr;

						return true;
					}

					sc::rx::subscription<T> * sub = this->subscriptions;
					while (sub != nullptr)
					{
						if (sub->next != sub && *(sub->next) == s)
						{
							sub->next = (*(s.next) != s) ? s.next : sub;
							return true;
						}

						sub = (sub->next != sub) ? sub->next : nullptr;
					}
				}
				return false;
			}

			virtual ~ObservableBase() = default;

		protected:
			subscription<T> * subscriptions;

		};

		template<class T>
		class Observable: public ObservableBase<T>
		{

		public:

			void next(T value)
			{
				subscription<T> * sub = this->subscriptions;
				while (sub != nullptr)
				{
					if (*sub)
					{
						(*sub)->next(value);
					}
					sub = (sub->next != sub) ? sub->next : nullptr;
				}
			}

			virtual ~Observable() = default;

		};

		template<>
		class Observable<void> : public ObservableBase<void>
		{

		public:
			void next()
			{
				subscription<void> * sub = this->subscriptions;
				while (sub != nullptr)
				{
					if (*sub)
					{
						(*sub)->next();
					}
					sub = (sub->next != sub) ? sub->next : nullptr;
				}
			}

			virtual ~Observable() = default;

		};

	} /* namespace sc::rx */
} /* namespace sc */

#endif /* SC_RXCPP_H_ */
